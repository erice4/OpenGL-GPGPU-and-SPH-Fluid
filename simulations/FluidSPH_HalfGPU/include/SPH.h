#ifndef SPH_H
#define SPH_H

#include <ParticleEmitter.h>
#include <Particle.h>
#include <memory>
#include "Octree.h"
#include "VectorField.h"
#include "ComputeShader.h"
#include <unordered_map>

const unsigned int MAX_NEIGHBOURS = 150;

//----------------------------------------------------------------------------------------------------------------------
/// @file SPH.h
/// @brief SPH implementation of fluid based off Clavet et. al http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.59.9379&rep=rep1&type=pdf
/// @author Ewan Rice
/// @version 1.0
/// @date 06/08/16
/// @class SPH
/// @brief Fluid calculations are carried out on the GPU across various compute shaders.
//----------------------------------------------------------------------------------------------------------------------

class SPH
{
  struct vec4
  {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
  };

  struct GLParticle
  {
    vec4 position;
    vec4 velocity;
    GLfloat density;
    GLfloat nearDensity;
    GLfloat m_pressure;
    GLfloat m_nearPressure;
    GLuint neighbours[MAX_NEIGHBOURS];
  };

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
private:

  void initialize();

  /// @brief the following section is based on Clavet et. al http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.59.9379&rep=rep1&type=pdf :-
  void applyExternalForces();
  void applyViscosity();
  void advection();
  void calculateDensityPressureDisplacement();
  void collisions();
  void calculateVelocity();
  /// end of citation

  void addParticlesToCells();
  void fillParticleNeighbours();

  void initializeBoundsVAO();
  void addBoundaryDrawData(BoundingBox _bounds);

  std::unique_ptr<ParticleEmitter> m_particleEmitter;
  std::unique_ptr<VectorField> m_vectorField;
  std::unique_ptr<Octree> m_octree;

  unsigned int m_numParticles;
  unsigned int m_numCells;
  unsigned int m_gridSize;

  float m_kernel;
  float m_linearViscosity;
  float m_quadraticViscosity;
  float m_stiffness;
  float m_nearStiffness;
  float m_restDensity;
  float m_particleRadius;
  float m_dt;

  bool m_2d;

  ngl::Vec3 m_gravity;
  ngl::Vec3 m_wind;

  BoundingBox m_bounds;

  ngl::VertexArrayObject *m_vao;
  std::vector<ngl::Vec3> m_drawData;

  doGPU::ComputeShader m_viscoCompute;
  doGPU::ComputeShader m_advectCompute;
  doGPU::ComputeShader m_densPressDispCompute;
  doGPU::ComputeShader m_velocityCompute;
  doGPU::ComputeShader m_collisionsCompute;

  std::unordered_map<std::string, doGPU::SSBO> m_ssbos;
};

#endif // SPH_H
