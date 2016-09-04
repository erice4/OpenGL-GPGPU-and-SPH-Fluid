#include "Octree.h"
#include <iostream>
#include <ngl/ShaderLib.h>

Octree::Octree(unsigned int _height, ngl::Vec3 _position, BoundingBox _bounds)
{
  m_rootNode = new TreeNode();

  _bounds.m_minx += _position.m_x+0.1f; _bounds.m_maxx += _position.m_x+0.1f;
  _bounds.m_miny += _position.m_y+0.1f; _bounds.m_maxy += _position.m_y+0.1f;
  _bounds.m_minz += _position.m_z+0.1f; _bounds.m_maxz += _position.m_z+0.1f;

  createTree(std::move(m_rootNode), _height, _bounds);
}

Octree::~Octree()
{
  deleteTreeNode(m_rootNode);
  m_vao->removeVOA();
}

void Octree::createTree(TreeNode* _parent, int _height, BoundingBox _bounds)
{
  _parent->m_bounds = _bounds;
  _parent->m_height = _height;
  _parent->m_particleList.clear();
  _parent->m_vectorNodeList.clear();

  _height--;

  if (_height==0) // leaves of the tree have been reached, set children to null
  {
    for (int i=0; i< 8; i++)
    {
      _parent->m_children[i] = 0;
    }

    addCubeLines(_bounds);
  }
  else //create 8 children, each with a volume 1/8 the parent bounds, and equally spaced from each other
  {
    BoundingBox childBounds;

    for (int i=0; i<8; i++)
    {
      _parent->m_children[i]=new TreeNode();

      if (i%2==0) //0,2,4,6 on left half
      {
        childBounds.m_minx = _bounds.m_minx;
        childBounds.m_maxx = (_bounds.m_maxx+_bounds.m_minx)/2.0f;
      }
      else //1,3,5,7 on right half
      {
        childBounds.m_minx = (_bounds.m_maxx+_bounds.m_minx)/2.0f;
        childBounds.m_maxx = _bounds.m_maxx;
      }

      if (i==0 || i==1 || i==4 || i==5) //Top half
      {
        childBounds.m_miny = _bounds.m_miny;
        childBounds.m_maxy = (_bounds.m_miny + _bounds.m_maxy)/2.0f;
      }
      else //Bottom Half
      {
        childBounds.m_miny = (_bounds.m_miny + _bounds.m_maxy)/2.0f;
        childBounds.m_maxy = _bounds.m_maxy;
      }

      if (i<4)  //Back half (I think)
      {
        childBounds.m_minz = _bounds.m_minz;
        childBounds.m_maxz = (_bounds.m_minz + _bounds.m_maxz) /2.0f;
      }
      else
      {
        childBounds.m_minz = (_bounds.m_minz + _bounds.m_maxz) /2.0f;
        childBounds.m_maxz = _bounds.m_maxz;
      }

      createTree(std::move(_parent->m_children[i]), _height, childBounds);
    }
  }

  initializeVAO();
}

void Octree::initializeVAO()
{
  m_vao=ngl::VertexArrayObject::createVOA(GL_LINES);
  m_vao->bind();
  m_vao->setData(m_drawData.size()*sizeof(ngl::Vec3),m_drawData[0].m_x);
  m_vao->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(ngl::Vec3),0);
  m_vao->setNumIndices(m_drawData.size());

  m_vao->unbind();

  glPointSize(10);
}

void Octree::deleteTreeNode(TreeNode* _node)
{
  for(int i=0;i<8;++i)
  {
    if(_node->m_children[i]!=0)
    {
      deleteTreeNode(_node->m_children[i]);
      delete _node->m_children[i];
    }
  }
}

bool Octree::checkIsOutsideBounds(const ngl::Vec3 &_objectPos, const BoundingBox &_treeNodeBounds)
{
  return ((_objectPos.m_x < _treeNodeBounds.m_minx || _objectPos.m_x > _treeNodeBounds.m_maxx
          || _objectPos.m_y < _treeNodeBounds.m_miny || _objectPos.m_y > _treeNodeBounds.m_maxy
          || _objectPos.m_z < _treeNodeBounds.m_minz || _objectPos.m_z > _treeNodeBounds.m_maxz));
}

void Octree::addParticleToTree(Particle *_p)
{
  addParticleToTreeNode(std::move(m_rootNode), _p);
}

void Octree::addParticleToTreeAndVectorNodes(Particle *_p)
{
  addParticleToTreeNodeAndVectorNodes(m_rootNode, _p);
}

void Octree::addVectorNode(VectorNode *_vecNode)
{
  addVectorNodeToTreeNode(m_rootNode,_vecNode);
}

void Octree::clearParticlesFromTreeNode(TreeNode* _node)
{
  if(_node->m_height == 1)
  {
    _node->m_particleList.clear();
  }
  else
  {
    for(int i=0;i<8;++i)
    {
        clearParticlesFromTreeNode(_node->m_children[i]);
    }
  }
}

void Octree::clearParticlesFromTreeNodeAndVectorNodes(TreeNode* _node)
{
  if(_node->m_height == 1)
  {
    for (unsigned long i=0; i<_node->m_vectorNodeList.size(); i++)
    {
      _node->m_vectorNodeList[i]->m_particleList.clear();
    }

    _node->m_particleList.clear();
  }
  else
  {
    for(int i=0;i<8;++i)
    {
        clearParticlesFromTreeNodeAndVectorNodes(_node->m_children[i]);
    }
  }
}

void Octree::clearVectorNodesFromTreeNode(TreeNode* _node)
{
  if(_node->m_height == 1)
  {
    _node->m_vectorNodeList.clear();
  }
  else
  {
    for(int i=0;i<8;++i)
    {
        clearVectorNodesFromTreeNode(_node->m_children[i]);
    }
  }
}

//Recusively run through child nodes of the parent passed in, finding if the particle position is within a node's bounds, if it is, then
//go into children of that node and continue to check if within the bounds. If that node is a leaf node, then add the particle.
void Octree::addParticleToTreeNode(TreeNode* _node, Particle *_p)
{
  if (_node->m_height == 1) //Leaf node found so add the particle
  {
    _node->m_particleList.push_back(_p);
  }
  else
  {
    for (int i=0; i<8; i++)
    {
      if (!checkIsOutsideBounds(_p->m_pos, _node->m_bounds))
      {
        addParticleToTreeNode(_node->m_children[i], _p);
      }
    }
  }
}

void Octree::addParticleToTreeNodeAndVectorNodes(TreeNode* _node, Particle *_p)
{

  if (_node->m_height == 1) //Leaf node found so add the particle
  {
    if (!checkIsOutsideBounds(_p->m_pos, _node->m_bounds))
    {

      _node->m_particleList.push_back(_p);

      for (unsigned long i=0; i<_node->m_vectorNodeList.size(); i++)
      {
        if (!_node->m_vectorNodeList[i]->checkIsOutsideBounds(_p->m_pos))
        {
          _node->m_vectorNodeList[i]->m_particleList.push_back(_p);
          break;
        }
      }
    }
  }
  else
  {
    for (int i=0; i<8; i++)
    {
      if (!checkIsOutsideBounds(_p->m_pos, _node->m_bounds))
      {
        addParticleToTreeNodeAndVectorNodes(_node->m_children[i], _p);
      }
    }
  }
}

void Octree::addVectorNodeToTreeNode(TreeNode* _node, VectorNode *_vecNode)
{
  if (_node->m_height == 1) //Leaf node found so add the vector node
  {
    if (!checkIsOutsideBounds(_vecNode->m_position, _node->m_bounds))
    {
      _node->m_vectorNodeList.push_back(_vecNode);
    }
  }
  else
  {
    for (int i=0; i<8; i++)
    {
      if (!checkIsOutsideBounds(_vecNode->m_position, _node->m_bounds))
      {
        addVectorNodeToTreeNode(_node->m_children[i], _vecNode);
      }
    }
  }
}

void Octree::draw(ngl::Mat4 _VP)
{
  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  (*shader)["Color"]->use();

  shader->setShaderParamFromMat4("VP",_VP);

  m_vao->bind();
  m_vao->draw();
  m_vao->unbind();
}

void Octree::addCubeLines(BoundingBox _bounds)
{
  ngl::Vec3 v1 = ngl::Vec3(_bounds.m_minx,_bounds.m_miny,_bounds.m_minz);
  ngl::Vec3 v2 = ngl::Vec3(_bounds.m_minx,_bounds.m_maxy,_bounds.m_minz);
  ngl::Vec3 v3 = ngl::Vec3(_bounds.m_minx,_bounds.m_miny,_bounds.m_maxz);
  ngl::Vec3 v4 = ngl::Vec3(_bounds.m_minx,_bounds.m_maxy,_bounds.m_maxz);

  ngl::Vec3 v5 = ngl::Vec3(_bounds.m_maxx,_bounds.m_miny,_bounds.m_minz);
  ngl::Vec3 v6 = ngl::Vec3(_bounds.m_maxx,_bounds.m_maxy,_bounds.m_minz);
  ngl::Vec3 v7 = ngl::Vec3(_bounds.m_maxx,_bounds.m_miny,_bounds.m_maxz);
  ngl::Vec3 v8 = ngl::Vec3(_bounds.m_maxx,_bounds.m_maxy,_bounds.m_maxz);

  //Drawing cube using lines (involves duplicate points)
  m_drawData.push_back(v1);m_drawData.push_back(v2); //1 -> 2
  m_drawData.push_back(v2);m_drawData.push_back(v4); //2 -> 4 ...
  m_drawData.push_back(v4);m_drawData.push_back(v3);
  m_drawData.push_back(v3);m_drawData.push_back(v1);

  m_drawData.push_back(v1);m_drawData.push_back(v5);
  m_drawData.push_back(v5);m_drawData.push_back(v7);
  m_drawData.push_back(v7);m_drawData.push_back(v3);

  m_drawData.push_back(v4);m_drawData.push_back(v8);
  m_drawData.push_back(v8);m_drawData.push_back(v6);
  m_drawData.push_back(v6);m_drawData.push_back(v2);

  m_drawData.push_back(v8);m_drawData.push_back(v7);

  m_drawData.push_back(v6);m_drawData.push_back(v5);
}
