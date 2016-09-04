#ifndef PARTICLEEMITTER_H__
#define PARTICLEEMITTER_H__

#include "Particle.h"
#include <ngl/Vec3.h>
#include "vector"
#include <ngl/Mat4.h>

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
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor, initializes ParticleEmitter and creates the particles.
  /// @param [in] _numberOfParticles Total number of particles to be created.
  /// @param [in] _emissionSourceSize Size of the cuboid from which to emit particles.
  /// @param [in] _particleLifeTime Initial lifetime of each particle.
  /// @param [in] _particleMass Mass of each particle.
  /// @param [in] _dragCoefficient Drag coefficient of each particle. May be thought of as air resistance. Recommended to keep value between 0 and 1.
  /// @param [in] _shaderName String name of the shader program used to draw the particles.
  ParticleEmitter(int _numberOfParticles, ngl::Vec3 _emissionSourceSize, float _particleLifeTime, float _particleMass, float _dragCoefficient, char* _shaderName);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Destructor
  ~ParticleEmitter();
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Change the position of the emisssion source. Changing this will move the initial positions of all particles in relation to this new value.
  /// @param [in] _position Position to set the particle emitter to.
  void setInitialPosition(ngl::Vec3 _position);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Set the gravity force applied to each particle
  /// @param [in] _gravity Gravity force, uniformly applied to every particle.
  void setGravity(ngl::Vec3 _gravity);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Set max number of particles emitted per frame
  /// @param [in] _emissionRate Position to set the particle emitter to.
  void setEmissionRate(float _emissionRate);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Change the max number of particles.
  /// @param [in] _numParticles Max number of particles.
  void setTotalParticles(int _numParticles);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Change the mass of each particle.
  /// @param [in] _mass Particle mass.
  void setParticleMass(float _mass);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Change the initial lifetime of each particle, the total time a particle will remain active.
  /// @param [in] _lifetime Particle lifetime.
  void setParticleLifetime(float _lifetime);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Set the drag coefficient of each particle. May be thought of as air resistance. Recommended to keep value between 0 and 1.
  /// @param [in] _drag Particle drag coefficient.
  void setDragCoefficient(float _drag);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Size of the cuboid from which to emit particles. Particles are randomly emitted from any position within this cuboid.
  /// @param [in] _size Size of the emission source cuboid.
  void setEmissionSourceSize(ngl::Vec3 _size);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Set a new initial position for all particles using a new random seed
  void setNewParticleInitialPosition();
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Max number of particles emitted per frame
  /// @param [in] _shaderName Position to set the particle emitter to.
  void setShaderName(char* _shaderName);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Called in update to emit new particles if there are any inactive particles available and depending on emission rate.
  void emitParticles();
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Call once per frame. This method calls update on each particle, which will update their position.
  /// @param [in] _dt Time step to be passed in. Can be a constant, or time between frames.
  void update(float _dt);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Call every frame that the scene is drawn. This method calls update on each particle, which will update their position.
  /// @param [in] _VP View, Projection matrix.
  /// @param [in] _cameraPosition Current position of the camera.
  void draw(ngl::Mat4 _VP, ngl::Vec3 _cameraPosition);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief List of particles
  std::vector<Particle> m_particles;
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Max number of particles.
  int m_maxParticles;
  //-----------------------------------------------------------------------------------------------------------------------------------
private:
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Create and return a new particle
  /// @param [in] _position Initial position of the particle.
  Particle createParticle(ngl::Vec3 _position);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Initial position of emitter.
  ngl::Vec3 m_initialPos;
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Gravity force applied to each particle.
  ngl::Vec3 m_gravity;
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Mass of each particle.
  float m_particleMass;
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Initial lifetime of each partile.
  float m_particleLifeTime;
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Drag coefficient of each particle.
  float m_dragCoefficient;
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Size of the emission area. Particles are randomly created at any position within this volume.
  ngl::Vec3 m_emissionSourceSize;
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Max number of particles emitted per frame.
  int m_emissionRate;
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief Next available active particle that can be emitted.
  /// The following section is from :-
  /// Robert Nystrom (2 Nov 2014). Game Programming Patterns [online]
  /// [Accessed 2015[. Available from: <http://gameprogrammingpatterns.com/object-pool.html>.
  Particle* m_firstAvailble;
  /// end of citation
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief String name of the shader program used to draw the particles.
  char* m_shaderName;
  //-----------------------------------------------------------------------------------------------------------------------------------
};

#endif // PARTICLEEMITTER_H__

