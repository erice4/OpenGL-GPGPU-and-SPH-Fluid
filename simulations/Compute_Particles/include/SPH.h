#ifndef SPH_H
#define SPH_H

#include <ParticleEmitter.h>
#include <Particle.h>
#include <memory>
#include "Octree.h"
#include "VectorField.h"

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
  /// @brief Constructor, pass in parameters for the fluid and particles
  /// @param [in] _numberParticles Number of particles
  /// @param [in] _viscosity A measure of how thick the fluid is, as in does it flow like oil or like water
  /// @param [in] _kernal Distance in which particles will be affected by other particles
  /// @param [in] _stiffness May be used to dampen the speed of particles
  /// @param [in] _nearStiffness Used to prevent particles clustering, assign a value larger than _stiffness
  /// @param [in] _restDensity The density of a particle at rest
  /// @param [in] _surfaceTension An extra parameter to control the speed of the particles in moving away or towards eachother.
  /// @param [in] _dt time step
  SPH(unsigned int _numberParticles, float _viscosity, float _kernel, float _stiffness, float _nearStiffness, float _restDensity, float _surfaceTension, float _dt);
  ~SPH();

  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Calls the fluid calculations and updates particle positions as well as adding particles to cells
  void update();
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Draw the bounding box of the grid
  void draw(ngl::Mat4 _VP, ngl::Vec3 _cameraPosition, GLuint _particleTextureID);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Reset the particles back to the initial positions
  void reset();
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief set the x component of a global wind vector
  void setWindX(float _x);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief set the x component of a global wind vector
  void setWindY(float _y);
  //-----------------------------------------------------------------------------------------------------------------------------------

private:

  void initialize();

  /// @brief the following section is based on the following code and modified :-
  /// https://imdoingitwrong.wordpress.com/2010/12/14/why-my-fluids-dont-flow/
  /// https://github.com/finallyjustice/sphfluid/tree/master/SPH_CPU_2D_v2

  void calculateDensityAndPressure();
  void calculateForce();
  void advection();

  /// end of citation

  void initializeBoundsVAO();
  void addBoundaryDrawData(BoundingBox _bounds);

  void particleCollision(Particle *p1, Particle *p2);

  /// @brief The following section is from :-
  /// https://github.com/finallyjustice/sphfluid/blob/master/SPH_CPU_2D_v2/SPHSystem.cpp
  float poly6(float f);
  float spiky(float f);
  float visco(float f);
  /// end of citation

  std::unique_ptr<ParticleEmitter> m_particleEmitter;
  std::unique_ptr<VectorField> m_vectorField;
  std::unique_ptr<Octree> m_octree;

  unsigned int m_numParticles;
  unsigned int m_numCells;
  unsigned int m_gridSize;

  float m_kernel;
  float m_viscosity;
  float m_stiffness;
  float m_nearStiffness;
  float m_surfaceTension;
  float m_restDensity;
  float m_particleRadius;
  float m_radX6;
  float m_dt, m_dtSquared;
  float m_norm;
  float m_nearNorm;

  ngl::Vec3 m_wind;

  BoundingBox m_bounds;

  ngl::VertexArrayObject *m_vao;
  std::vector<ngl::Vec3> m_drawData;
};

#endif // SPH_H
