#ifndef PARTICLE_H__
#define PARTICLE_H__

#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <memory>

//----------------------------------------------------------------------------------------------------------------------
/// @file Particle.h
/// @brief This class represents a particle. It stores necessary data for drawing and updating a particle and also handles the integration of the particle.
/// @author Ewan Rice
/// @version 1.0
/// @date 28/01/16
/// @class Particle
/// @brief This class represents a particle. It stores necessary data for drawing and updating a particle and also handles the integration of the particle.
/// The object pooling portion of this class was written based off the following :-
/// Robert Nystrom (2 Nov 2014). Game Programming Patterns [online]
/// [Accessed 2015[. Available from: <http://gameprogrammingpatterns.com/object-pool.html>.
//----------------------------------------------------------------------------------------------------------------------

class Particle
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief constructor
  Particle();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Destructor
  ~Particle();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set initial position the particle will be emitted from.
  /// @param [in] _position Initial position.
  void setInitialPosition(ngl::Vec3 _position);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set initial velocity of the particle on emission.
  /// @param [in] _velocity Initial velocity.
  void setInitialVelocity(ngl::Vec3 _velocity);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set initial lifetime of the particle on emission.
  /// @param [in] _lifeTime Initial lifetime.
  void setInitialLifeTime(float _lifeTime);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set the particle mass.
  /// @param [in] _mass Particle mass.
  void setMass(float _mass);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set drag coefficient of the particle. May be thought of as air resistance. Recommended to keep value between 0 and 1.
  /// @param [in] _coefficient Drag coefficient.
  void setDragCoefficient(float _coefficient);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Update the particle. This will update particle position and lifetime.
  /// @param [in] _dt Time step.
  /// @param [in] _gravity Gravity force applied to the particle.
  bool update(float _dt, ngl::Vec3 _gravity);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Reset the particle's intial values and set to active.
  void reset();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Return the pointer to the next available active Particle.
  Particle* getNext() {return m_next;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set the pointer for the next available active Particle.
  void setNext(Particle* _next) {m_next = _next;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Add a force to the particle. This force will be applied in the update method.
  void addForce(ngl::Vec3 &_force);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Return the current lifetime of the particle.
  float getLifetime(){return m_lifeTime;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Return the initial velocity of the particle.
  ngl::Vec3 getInitialVelocity(){return m_initialVelocity;}
  //----------------------------------------------------------------------------------------------------------------------

  bool m_active;
  ngl::Vec3 m_velocity;
  ngl::Vec3 m_smoothVelocity;
  ngl::Vec3 m_color;
  ngl::Vec3 m_pos;
  ngl::Vec3 m_relaxedPos;
  float m_mass;
  float m_dragCoefficient;
  ngl::Vec3 m_initialPos;
  float m_initialLifeTime;
  float m_density;
  float m_nearDensity;
  float m_pressure;
  float m_nearPressure;
  ngl::Vec3 m_force;

private:

  ngl::Vec3 m_initialVelocity;
  float m_lifeTime;

  /// @brief The following section is from :-
  /// Robert Nystrom (2 Nov 2014). Game Programming Patterns [online]
  /// [Accessed 2015[. Available from: <http://gameprogrammingpatterns.com/object-pool.html>.
  Particle* m_next;
  /// end of citation
};

#endif // PARTICLE_H__

