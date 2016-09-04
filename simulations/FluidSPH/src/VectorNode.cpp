#include "VectorNode.h"
#include <iostream>


VectorNode::VectorNode(ngl::Vec3 _position, ngl::Vec3 _vector, ngl::Vec3 _cellSize)
{
  m_position = _position;
  m_vector = _vector;
  m_cellSize = _cellSize;

  m_bounds.m_minx = m_position.m_x - m_cellSize.m_x/2.0f; m_bounds.m_maxx = m_position.m_x + m_cellSize.m_x/2.0f;
  m_bounds.m_miny = m_position.m_y - m_cellSize.m_y/2.0f; m_bounds.m_maxy = m_position.m_y + m_cellSize.m_y/2.0f;
  m_bounds.m_minz = m_position.m_z - m_cellSize.m_z/2.0f; m_bounds.m_maxz = m_position.m_z + m_cellSize.m_z/2.0f;
}

VectorNode::~VectorNode()
{

}

void VectorNode::setPosition(ngl::Vec3 _position)
{
  m_position = _position;
}

void VectorNode::setVector(ngl::Vec3 _vector)
{
  m_vector = _vector;
}

void VectorNode::setCellSize(ngl::Vec3 _cellSize)
{
  m_cellSize = _cellSize;
}

void VectorNode::addParticle(Particle* _p)
{
  m_particleList.push_back(_p);
}

void VectorNode::clearParticles()
{
  m_particleList.clear();
}

/// @brief Originally written by Jon Macey
/// https://github.com/NCCA/OctreeAbstract
bool VectorNode::checkIsOutsideBounds(const ngl::Vec3 &_objectPos)
{
  return (_objectPos.m_x < m_bounds.m_minx || _objectPos.m_x > m_bounds.m_maxx
          || _objectPos.m_y < m_bounds.m_miny || _objectPos.m_y > m_bounds.m_maxy
          || _objectPos.m_z < m_bounds.m_minz || _objectPos.m_z > m_bounds.m_maxz);
}
/// end of citation

void VectorNode::addForceToParticles(float &_magnitude)
{
  ngl::Vec3 f = m_vector*_magnitude;
  for (unsigned long i=0; i<m_particleList.size(); i++)
  {
    m_particleList[i]->addForce(f);
  }
}
