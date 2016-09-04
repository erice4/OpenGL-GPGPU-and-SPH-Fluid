#ifndef OCTREE_H__
#define OCTREE_H__

#include <ngl/Vec3.h>
#include <ngl/VertexArrayObject.h>
#include <Particle.h>
#include <VectorNode.h>
#include <vector>
#include <memory>
#include <array>

//----------------------------------------------------------------------------------------------------------------------
/// @file Octree.h
/// @brief This class provides an efficient method to add particles to Vector Nodes
/// @author Ewan Rice
/// @version 1.0
/// @date 28/01/16
/// @class Octree
/// @brief This class provides an efficient method to add particles to Vector Nodes.
/// This class was originally written by Jon Macey and modified for use in this program.
/// https://github.com/NCCA/OctreeAbstract/blob/master/include/AbstractOctree.h
//----------------------------------------------------------------------------------------------------------------------

/// @brief Class to represent the boundaries of a volume.
class BoundingBox
{
public :
    ngl::Real m_minx;
    ngl::Real m_maxx;
    ngl::Real m_miny;
    ngl::Real m_maxy;
    ngl::Real m_minz;
    ngl::Real m_maxz;
};



class Octree
{

  /// @brief A tree node that holds a list of particles and vector nodes that currently are positioned inside of it's bounding box.
  /// These nodes are created at every level of the tree.
  class TreeNode
  {
    public:
      BoundingBox m_bounds;
      int m_height;
      std::vector<Particle*> m_particleList;
      std::vector<VectorNode*> m_vectorNodeList;
      std::array<TreeNode*,8> m_children;
  };


public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief constructor
  /// @param [in] _height the number of levels to the tree
  /// @param [in] _position the center point of the tree in world space
  /// @param [in] _bounds the boundaries of the root node, as in the total boundary of the tree
  Octree(unsigned int _height, ngl::Vec3 _position, BoundingBox _bounds);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief destructor
  //----------------------------------------------------------------------------------------------------------------------
  ~Octree();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief delete a node and all of it's children nodes
  //----------------------------------------------------------------------------------------------------------------------
  void deleteTreeNode(TreeNode* _node);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief add a particle to the octree. If this particle is found to be positioned within the boundary of a leaf node, the particle will be added to that leaf node.
  //----------------------------------------------------------------------------------------------------------------------
  void addParticleToTree(Particle* _p);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief add a particle to the octree and
  //----------------------------------------------------------------------------------------------------------------------
  void addParticleToTreeAndVectorNodes(Particle* _p);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set up all shaders to be used
  //----------------------------------------------------------------------------------------------------------------------
  void addVectorNode(VectorNode *_vecNode);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set up all shaders to be used
  //----------------------------------------------------------------------------------------------------------------------
  void clearParticlesFromTreeNode(TreeNode* _node);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set up all shaders to be used
  //----------------------------------------------------------------------------------------------------------------------
  void clearParticlesFromTreeNodeAndVectorNodes(TreeNode* _node);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set up all shaders to be used
  //----------------------------------------------------------------------------------------------------------------------
  void clearVectorNodesFromTreeNode(TreeNode* _node);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief draw the bounds of the leaf nodes of the octree
  void draw(ngl::Mat4 _VP);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Root tree node
  //----------------------------------------------------------------------------------------------------------------------
  TreeNode* m_rootNode;

private:
  /// @brief create the tree
  /// @param [in] _parent parent node
  //----------------------------------------------------------------------------------------------------------------------
  void createTree(TreeNode* _parent, int _height, BoundingBox _bounds);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set up
  //----------------------------------------------------------------------------------------------------------------------
  void initializeVAO();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set up all shaders to be used
  //----------------------------------------------------------------------------------------------------------------------
  bool checkIsOutsideBounds(const ngl::Vec3 &_objectPos, const BoundingBox &_treeNodeBounds);

  void addParticleToTreeNode(TreeNode* _node, Particle *_p);
  void addParticleToTreeNodeAndVectorNodes(TreeNode* _node, Particle *_p);
  void addVectorNodeToTreeNode(TreeNode* _node, VectorNode *_vecNode);

  void addCubeLines(BoundingBox _bounds);


  ngl::VertexArrayObject *m_vao;
  std::vector<ngl::Vec3> m_drawData;

};

#endif // OCTREE_H__

