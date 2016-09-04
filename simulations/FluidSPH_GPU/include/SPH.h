#ifndef SPH_H
#define SPH_H

#include <ParticleEmitter.h>
#include <Particle.h>
#include <memory>
#include "ComputeShader.h"
#include <unordered_map>
#include <ngl/VertexArrayObject.h>

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
  #pragma pack(push,1)
  struct vec4
  {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
  };
  #pragma pack(pop)

  struct Sphere
  {
    ngl::Vec3 m_position;
    float m_radius;
  };

public:
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor, sets default values
  /// @param [in] _dt time step
  SPH(float _dt);
  //-----------------------------------------------------------------------------------------------------------------------------------
  ~SPH();

  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Invocates the compute shaders
  void update();
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Draw the bounding box of the grid
  void draw(ngl::Mat4 _VP, ngl::Vec3 _cameraPosition, GLuint _particleTextureID);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Reset the particles back to the initial positions
  void reset();
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

  void initializeBoundsVAO();

  unsigned int m_numParticles;
  unsigned int m_numCells;
  unsigned int m_gridSize;
  ngl::Vec3 m_gridCenter;
  ngl::Vec3 m_gridOffset;

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

  ngl::VertexArrayObject *m_vao;
  std::vector<ngl::Vec3> m_drawData;

  doGPU::ComputeShader m_viscoCompute;
  doGPU::ComputeShader m_advectCompute;
  doGPU::ComputeShader m_clearGridCompute;
  doGPU::ComputeShader m_addToGridCompute;
  doGPU::ComputeShader m_densPressDispCompute;
  doGPU::ComputeShader m_collisionsCompute;
  doGPU::ComputeShader m_testCompute;

  std::unordered_map<std::string, doGPU::SSBO> m_ssbos;
};

#endif // SPH_H
