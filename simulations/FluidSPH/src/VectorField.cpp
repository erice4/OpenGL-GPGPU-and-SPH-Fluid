#include "VectorField.h"
#include <ngl/VertexArrayObject.h>
#include <ngl/Mat4.h>
#include <ngl/ShaderLib.h>
#include <iostream>

VectorField::VectorField(ngl::Vec3 _centerPosition, int _width, int _height, int _depth, ngl::Vec3 _cellSize)
{
  m_vectorMagnitude = 1; // default

  m_numCellsWidth = _width;
  m_numCellsHeight = _height;
  m_numCellsDepth = _depth;

  float fieldWidth = _width * _cellSize.m_x;
  float fieldDepth = _depth * _cellSize.m_y;
  float fieldHeight = _height * _cellSize.m_z;
  ngl::Vec3 startCellPos = ngl::Vec3(_centerPosition.m_x - fieldWidth/2.0f +0.1f, _centerPosition.m_y - fieldHeight/2.0f+0.1f, _centerPosition.m_z - fieldDepth/2.0f+0.1f);

  for (int k=0; k<_height; k++)
  {
    for (int j=0; j<_depth; j++)
    {
      for (int i=0; i<_width; i++)
      {      
        ngl::Vec3 pos = startCellPos;
        pos.m_x += _cellSize.m_x * i;
        pos.m_y += _cellSize.m_y * k;
        pos.m_z += _cellSize.m_z * j;

        ngl::Vec3 vec = getVectorValueFromFunction(VectorFunction::turbulentWind, pos);

        VectorNode node(pos, vec,_cellSize);
        VectorNode::CellIndex c;
        c.x = i;
        c.y = j;
        c.z = k;
        node.m_cellIndex = c;

        m_nodeList.push_back(node);
      }
    }
  }

  setupVAO();
}

VectorField::~VectorField()
{
  m_vao->removeVOA();
}

void VectorField::setupVAO()
{
  std::vector <drawData> nodesData = getDrawData();

  m_vao=ngl::VertexArrayObject::createVOA(GL_POINTS);
  m_vao->bind();
  m_vao->setData(nodesData.size()*sizeof(drawData),nodesData[0].pos.m_x);
  m_vao->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(drawData),0);
  m_vao->setVertexAttributePointer(1,3,GL_FLOAT,sizeof(drawData),sizeof(ngl::Vec3));
  m_vao->setVertexAttributePointer(2,3,GL_FLOAT,sizeof(drawData),sizeof(ngl::Vec3)*2);
  m_vao->setNumIndices(nodesData.size());

  m_vao->unbind();
}

std::vector<VectorField::drawData> VectorField::getDrawData()
{
  std::vector <drawData> nodesData;

  for(int i=0; i<m_numCellsWidth; i++)
  {
    for (int j=0; j<m_numCellsDepth; j++)
    {
      int index = i + (j*m_numCellsWidth) + ((m_numCellsHeight/2)*m_numCellsWidth*m_numCellsDepth);
      index = i + (j*m_numCellsWidth) + (0*m_numCellsWidth*m_numCellsDepth);
      drawData n;
      n.pos = m_nodeList[index].m_position;
      n.vector = m_nodeList[index].m_vector;
      n.color = ngl::Vec3(0,1,0);
      nodesData.push_back(n);
    }
  }

  for(int i=0; i<m_numCellsHeight; i++)
  {
    for (int j=0; j<m_numCellsDepth; j++)
    {
      int index = m_numCellsWidth/2 + (j*m_numCellsWidth) + (i*m_numCellsWidth*m_numCellsDepth);
      index = 0 + (j*m_numCellsWidth) + (i*m_numCellsWidth*m_numCellsDepth);
      drawData n;
      n.pos = m_nodeList[index].m_position;
      n.vector = m_nodeList[index].m_vector;
      n.color = ngl::Vec3(0,1,0);
      nodesData.push_back(n);
    }
  }

  for(int i=0; i<m_numCellsHeight; i++)
  {
    for (int j=0; j<m_numCellsWidth; j++)
    {
      int index = j + (m_numCellsDepth/2*m_numCellsWidth) + (i*m_numCellsWidth*m_numCellsDepth);
      index = j + (0*m_numCellsWidth) + (i*m_numCellsWidth*m_numCellsDepth);
      drawData n;
      n.pos = m_nodeList[index].m_position;
      n.vector = m_nodeList[index].m_vector;
      n.color = ngl::Vec3(0,1,0);
      nodesData.push_back(n);
    }
  }

  return nodesData;
}

void VectorField::setVectorMagnitude(float _magnitude)
{
  m_vectorMagnitude = _magnitude;
}

void VectorField::setGlobalVector(ngl::Vec3 _v)
{
  for (unsigned long i=0; i<m_nodeList.size(); i++)
  {
    m_nodeList[i].m_vector = _v;
  }
}

void VectorField::setVectorFunction(unsigned int _i)
{
  VectorFunction func;

  switch (_i)
  {
  case 1:
    func = VectorFunction::turbulentWind;
    break;
  case 2:
    func = VectorFunction::cyclone;
    break;
  case 3:
    func = VectorFunction::sinwave;
    break;
  case 4:
    func = VectorFunction::swirly;
    break;
  case 5:
    func = VectorFunction::sideways;
    break;
  default:
    func = VectorFunction::turbulentWind;

  }

  for (unsigned long i=0; i<m_nodeList.size(); i++)
  {
    m_nodeList[i].m_vector = getVectorValueFromFunction(func, m_nodeList[i].m_position);
  }

  std::vector <drawData> nodesData = getDrawData();
  m_vao->updateData(nodesData.size()*sizeof(drawData),nodesData[0].pos.m_x);
}

ngl::Vec3 VectorField::getVectorValueFromFunction(VectorField::VectorFunction _functionType, ngl::Vec3 _position)
{
  ngl::Vec3 vec = ngl::Vec3(0,0,0); 

  switch (_functionType)
  {
  case VectorFunction::cyclone:
    vec = cycloneFunction(_position);
    break;
  case VectorFunction::turbulentWind:
    vec = turbulentWindFunction(_position);
    break;
  case VectorFunction::sinwave:
    vec = sinwaveFunction(_position);
    break;
  case VectorFunction::swirly:
    vec = swirlyFunction(_position);
    break;
  case VectorFunction::sideways:
    vec = sidewaysFunction();
    break;
  }
  vec.normalize();

  return vec;
}

ngl::Vec3 VectorField::cycloneFunction(ngl::Vec3 _position)
{
  ngl::Vec3 v = ngl::Vec3(-_position.m_z, 0.0f, _position.m_x);
  return v;
}

ngl::Vec3 VectorField::turbulentWindFunction(ngl::Vec3 _position)
{
  ngl::Vec3 v = ngl::Vec3(1, sin(pow(_position.m_x,2) - pow(_position.m_y, 2)), 1);
  return v;
}

ngl::Vec3 VectorField::sinwaveFunction(ngl::Vec3 _position)
{
  ngl::Vec3 v = ngl::Vec3(1, sin(_position.m_x), 1);
  return v;
}

ngl::Vec3 VectorField::swirlyFunction(ngl::Vec3 _position)
{
  ngl::Vec3 v = ngl::Vec3(cos(_position.m_x + 2*_position.m_z), 0, sin(_position.m_x + 2*_position.m_z));
  return v;
}

ngl::Vec3 VectorField::sidewaysFunction()
{
  ngl::Vec3 v = ngl::Vec3(0.6f, 0.1f, 0.3f);
  return v;
}

void VectorField::applyVector()
{
  for (unsigned long i=0; i<m_nodeList.size(); i++)
  {
    m_nodeList[i].addForceToParticles(m_vectorMagnitude);
  }
}

void VectorField::draw(ngl::Mat4 _VP)
{
  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  (*shader)["Vector"]->use();

  shader->setShaderParamFromMat4("VP",_VP);

  m_vao->bind();
  m_vao->draw();
  m_vao->unbind();
}
