#ifndef SPH_H
#define SPH_H

#include <ParticleEmitter.h>
#include <Particle.h>
#include <memory>
#include "Octree.h"
#include "VectorField.h"
#include "Primitives.h"

//----------------------------------------------------------------------------------------------------------------------
/// @file SPH.h
/// @brief Applies fluid-like movement to particles by calculating their density and pressure in relation to surrounding particles and using that to calculate the force applied to a particle.
/// Calculations are based off the Navier Stokes equation. This class also creates instances of a vector field and octree to handle the spatial awareness of the particles in relation to each other and for optimization.
/// @author Ewan Rice
/// @version 1.0
/// @date 18/05/16
/// @class SPH
/// @brief The pressure, density and force calculation (Navier Stokes) portion of this class was written based off the following :-
/// https://imdoingitwrong.wordpress.com/2010/12/14/why-my-fluids-dont-flow/
/// https://github.com/finallyjustice/sphfluid/tree/master/SPH_CPU_2D_v2
//----------------------------------------------------------------------------------------------------------------------

class SPH
{
public:
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor, sets default values
  /// @param [in] _dt time step
  SPH(float _dt);
  //-----------------------------------------------------------------------------------------------------------------------------------
 ~SPH();

  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Calls the fluid calculations and updates particle positions as well as adding particles to cells
  void update();
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Draw the bounding box of the grid
  void draw(ngl::Mat4 _VP, ngl::Mat4 _V, GLuint _particleTextureID);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Reset the particles back to the initial positions
  void reset();
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Call when there is a left or right mouse click press
  void mouseClick(bool _leftMouse, float _x, float _y);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Call when there is a left or right mouse click release
  void mouseRelease(bool _leftMouse);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief set the x component of a global wind vector
  void setWindX(float _x);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief set the x component of a global wind vector
  void setWindY(float _y);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief set the kernel length
  void setKernel(float _v);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief set the rest density
  void setRestDensity(float _v);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief set the linear viscosity
  void setLinViscosity(float _v);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief set the quadratic viscosity
  void setQuadViscosity(float _v);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief set the stiffness
  void setStiffness(float _v);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief set the near stiffness
  void setNearStiffness(float _v);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief set the near stiffness
  void setGravity(float _v);
  //-----------------------------------------------------------------------------------------------------------------------------------

  bool m_2d;

private:

  void initialize();

  /// @brief the following section is based on Clavet et. al http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.59.9379&rep=rep1&type=pdf :-
  void applyExternalForces();
  void applyViscosity();
  void advection();
  void calculateDensityPressureDisplacement();
  void calculateDensityPressureDisplacement2D();
  void pressureDisplacement(Particle *p);
  void collisions();
  void calculateVelocity();
  /// end of citation

  void addParticlesToCells();

  void initializeBoundsVAO();
  void addBoundaryDrawData(std::vector<ngl::Vec3> &dataList, BoundingBox _bounds);
  void addRectangleDrawData(std::vector<ngl::Vec3> &dataList, Primitives::Rectangle &_r);
  void addCuboidDrawData(std::vector<ngl::Vec3> &dataList, Primitives::Cuboid &_c);

  void particleSphereCollision(Primitives::Sphere *_s, Particle *_p);
  void infinitePlaneCollision(Primitives::Plane *w, Particle* _p);
  void finitePlaneCollision(Primitives::Rectangle *_w, Particle* _p);
  void cuboidCollision(Primitives::Cuboid *_c, Particle* _p);

  void rotateCuboid(Primitives::Cuboid &_c, float _deg);

  std::unique_ptr<ParticleEmitter> m_particleEmitter;
  std::unique_ptr<VectorField> m_vectorField;
  std::unique_ptr<Octree> m_octree;

  unsigned int m_numParticles;
  unsigned int m_numCells;
  unsigned int m_gridSize;
  unsigned int m_gridZSize;

  float m_kernel;
  float m_linearViscosity;
  float m_quadraticViscosity;
  float m_stiffness;
  float m_nearStiffness;
  float m_restDensity;
  float m_particleRadius;
  float m_dt;
  float m_bounce;

  ngl::Vec3 m_gravity;
  ngl::Vec3 m_wind;
  ngl::Vec3 m_mousePos;

  float m_mouseClickForce = 100.2f;
  float m_currentClickForce;
  float m_clickRadius = 5;

  BoundingBox m_bounds;
  float m_distEdgeToCentreSquared;

  ngl::VertexArrayObject *m_vao;
  std::vector<ngl::Vec3> m_staticDrawData;

  std::vector<Primitives::Sphere> m_sphereList;
  std::vector<Primitives::Plane> m_wallList;
  std::vector<Primitives::Rectangle> m_rectangleList;
  std::vector<Primitives::Cuboid> m_cuboidList;
};

#endif // SPH_H
