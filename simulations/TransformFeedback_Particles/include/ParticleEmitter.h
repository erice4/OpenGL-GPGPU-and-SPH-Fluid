#ifndef PARTICLEEMITTER_H__
#define PARTICLEEMITTER_H__

#include "Particle.h"
#include <ngl/Vec3.h>
#include "vector"
#include <ngl/Mat4.h>
#include <ShaderManager.h>
#include <TransformFeedback.h>

//----------------------------------------------------------------------------------------------------------------------
/// @file ParticleEmitter.h
/// @brief Creates particles and acts as an interface for updating particle values. Handles the drawing of particles and calls their update method.
/// @author Ewan Rice
/// @version 1.0
/// @date 28/01/16
/// @class ParticleEmitter
/// @brief Creates particles and acts as an interface for updating particle values. Handles the drawing of particles and calls their update method.
/// The object pooling portion of this class was written based off the following :-
/// Robert Nystrom (2 Nov 2014). Game Programming Patterns [online]
/// [Accessed 2015]. Available from: <http://gameprogrammingpatterns.com/object-pool.html>.
//----------------------------------------------------------------------------------------------------------------------

class ParticleEmitter
{
public:

  struct vec2
  {
    GLfloat x;
    GLfloat y;
  };

  struct vec4
  {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
  };

  struct Particle
  {
    vec4 position;
    vec4 initialPosition;
    vec4 velocity;
    vec2 lifetime;
  };


  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor, initializes ParticleEmitter and creates the particles.
  /// @param [in] _numberOfParticles Total number of particles to be created.
  /// @param [in] _emissionSourceSize Size of the cuboid from which to emit particles.
  /// @param [in] _particleLifeTime Initial lifetime of each particle.
  /// @param [in] _particleMass Mass of each particle.
  /// @param [in] _dragCoefficient Drag coefficient of each particle. May be thought of as air resistance. Recommended to keep value between 0 and 1.
  /// @param [in] _shaderName String name of the shader program used to draw the particles.
  ParticleEmitter();
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Destructor
  ~ParticleEmitter();
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Initialize
  void initialize();
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Call once per frame. This method calls update on each particle, which will update their position.
  /// @param [in] _dt Time step to be passed in. Can be a constant, or time between frames.
  void update();
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Call every frame that the scene is drawn. This method calls update on each particle, which will update their position.
  /// @param [in] _VP View, Projection matrix.
  /// @param [in] _cameraPosition Current position of the camera.
  void draw(ngl::Mat4 _VP, ngl::Vec3 _cameraPosition);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Max number of particles.
  int m_maxParticles;
  //-----------------------------------------------------------------------------------------------------------------------------------
private:
  std::vector<Particle> m_particles;
  doGPU::TransformFeedback m_feedback;
};

#endif // PARTICLEEMITTER_H__

