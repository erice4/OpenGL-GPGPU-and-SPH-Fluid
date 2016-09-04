#ifndef VECTORNODE_H__
#define VECTORNODE_H__

#include <ngl/Vec3.h>
#include <Particle.h>
#include <vector>

//----------------------------------------------------------------------------------------------------------------------
/// @file VectorNode.h
/// @brief Represents a vector node. Used to apply a force to particles currently positioned inside it's bounds. Holds data for a node's vector, position, bounding box and a list of particles currently positioned inside it's bounds.
/// @author Ewan Rice
/// @version 1.0
/// @date 28/01/16
/// @class VectorNode
/// @brief Represents a vector node. Used to apply a force to particles currently positioned inside it's bounds. Holds data for a node's vector, position, bounding box and a list of particles currently positioned inside it's bounds.
//----------------------------------------------------------------------------------------------------------------------

/// @brief Class to represent the boundaries of a volume.
class Bounds
{
public :
    ngl::Real m_minx;
    ngl::Real m_maxx;
    ngl::Real m_miny;
    ngl::Real m_maxy;
    ngl::Real m_minz;
    ngl::Real m_maxz;
};
//----------------------------------------------------------------------------------------------------------------------

class VectorNode
{

public:

  /// @brief Constructor
  /// @param [in] _position Center position of the vector node.
  /// @param [in] _vector Vector. Represents the direction of the force applied to the particles.
  /// @param [in] _cellSize Size of the node.
  VectorNode(ngl::Vec3 _position, ngl::Vec3 _vector, ngl::Vec3 _cellSize);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Destructor
  ~VectorNode();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set the center position of the node.
  /// @param [in] _position Center position of the vector node.
  void setPosition(ngl::Vec3 _position);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set the center position of the node.
  /// @param [in] _vector Represents the direction of the force applied to the particles.
  void setVector(ngl::Vec3 _vector);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set the center position of the node.
  /// @param [in] _size Size of the node.
  void setCellSize(ngl::Vec3 _size);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Add a pointer to a particle to the list stored in the node.
  /// @param [in] _p Pointer to a particle to be added.
  void addParticle(Particle *_p);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Clear all particles in the list.
  void clearParticles();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Check if a position is inside the bounds of the node.
  /// @param [in] _objectPos Position to check.
  /// Originally written by Jon Macey
  bool checkIsOutsideBounds(const ngl::Vec3 &_objectPos);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Add force to all particles in the list.
  /// @param [in] Magnitude of the force.
  void addForceToParticles(float &_magnitude);
  //----------------------------------------------------------------------------------------------------------------------

  ngl::Vec3 m_position;
  ngl::Vec3 m_vector;
  ngl::Vec3 m_cellSize;

  std::vector<Particle*> m_particleList;

private:

  Bounds m_bounds;
};

#endif // VECTORNODE_H__

