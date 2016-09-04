#include "Particle.h"
#include <iostream>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <math.h>

Particle::Particle()
{
  m_active = true;
}

Particle::~Particle()
{
}

void Particle::addForce(ngl::Vec3 &_force)
{
  m_force+=_force;
}

void Particle::setInitialPosition(ngl::Vec3 _position)
{
  m_initialPos = _position;
  m_pos = _position;
}

void Particle::setInitialVelocity(ngl::Vec3 _velocity)
{
  m_initialVelocity = _velocity;
  m_velocity = _velocity;
}

void Particle::setInitialLifeTime(float _lifeTime)
{
  m_initialLifeTime = _lifeTime;
  m_lifeTime = _lifeTime;
}

void Particle::setMass(float _mass)
{
  m_mass = _mass;
}

void Particle::setDragCoefficient(float _coefficient)
{
  m_dragCoefficient = _coefficient;
}

bool::Particle::update(float _dt, ngl::Vec3 _gravity)
{  
  m_force += _gravity;

  //drag
  m_force += -m_dragCoefficient * m_velocity;

  ngl::Vec3 acc;
  //acc = m_force / (m_mass);

  acc = m_force;
  m_pos += m_velocity * _dt;

  m_velocity += _dt * acc;

  m_lifeTime -= _dt;

  m_force = ngl::Vec3(0,0,0);

  if (m_lifeTime <= 0)
  {
    m_active = false;
    return false;
  }

  return true;
}

void Particle::reset()
{
  m_pos = m_initialPos;
  m_previousPos = m_initialPos;
  m_density = 0;
  m_nearDensity = 0;
  m_pressure = 0;
  m_nearPressure = 0;
  m_velocity = m_initialVelocity;
  m_lifeTime = m_initialLifeTime;
  m_active = true;
}
