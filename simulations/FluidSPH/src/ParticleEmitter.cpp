#include "ParticleEmitter.h"
#include "stdlib.h"
#include <ctime>
#include <iostream>
#include <vector>
#include <ngl/Vec4.h>
#include <ngl/VertexArrayObject.h>
#include <ngl/Mat4.h>
#include <ngl/ShaderLib.h>
#include <ngl/Texture.h>
#include <random>

ParticleEmitter::ParticleEmitter(int _numberOfParticles, ngl::Vec3 _emissionSourceSize, float _particleLifeTime, float _particleMass, float _dragCoefficient, char* __shaderName)
{
  m_shaderName = __shaderName;

  m_maxParticles = _numberOfParticles;

  m_emissionSourceSize = _emissionSourceSize;
  m_particleLifeTime = _particleLifeTime;
  m_particleMass = _particleMass;
  m_dragCoefficient = _dragCoefficient;

  std::default_random_engine generator;
  generator.seed(time(0));

  std::uniform_real_distribution<float> distributionX(m_initialPos.m_x-m_emissionSourceSize.m_x/2.0f,m_initialPos.m_x+m_emissionSourceSize.m_x/2.0f);
  std::uniform_real_distribution<float> distributionY(m_initialPos.m_y-m_emissionSourceSize.m_y/2.0f,m_initialPos.m_y+m_emissionSourceSize.m_y/2.0f);
  std::uniform_real_distribution<float> distributionZ(m_initialPos.m_z-m_emissionSourceSize.m_z/2.0f,m_initialPos.m_z+m_emissionSourceSize.m_z/2.0f);

  for (int i=0; i<m_maxParticles; i++)
  {
    Particle p;
    p.setInitialPosition(ngl::Vec3(distributionX(generator), distributionY(generator), distributionZ(generator)));
    p.setInitialVelocity(ngl::Vec3(0,0,0));
    p.setInitialLifeTime(m_particleLifeTime);
    p.setMass(m_particleMass);
    p.m_color = ngl::Vec3(0.9f,1,1);
    p.setDragCoefficient(m_dragCoefficient);
    m_particles.push_back(p);
  }

  m_firstAvailble = &m_particles[0];

  for (int i=0; i<m_maxParticles-1; i++)
  {
    m_particles[i].setNext(&m_particles[i+1]);
  }

  m_particles[m_maxParticles-1].setNext(NULL);
}

Particle ParticleEmitter::createParticle(ngl::Vec3 _position)
{
  Particle p;
  p.setInitialPosition(ngl::Vec3(_position.m_x, _position.m_y, _position.m_z));
  p.setInitialVelocity(ngl::Vec3(0,0,0));
  p.setInitialLifeTime(m_particleLifeTime);
  p.setMass(m_particleMass);
  p.m_color = ngl::Vec3(0.9f,1,1);
  p.setDragCoefficient(m_dragCoefficient);

  return p;
}

ParticleEmitter::~ParticleEmitter()
{

}

void ParticleEmitter::setInitialPosition(ngl::Vec3 _initialPosition)
{
  m_initialPos = _initialPosition;

  std::default_random_engine generator;
  std::uniform_real_distribution<float> distributionX(m_initialPos.m_x-m_emissionSourceSize.m_x/2.0f,m_initialPos.m_x+m_emissionSourceSize.m_x/2.0f);
  std::uniform_real_distribution<float> distributionY(m_initialPos.m_y-m_emissionSourceSize.m_y/2.0f,m_initialPos.m_y+m_emissionSourceSize.m_y/2.0f);
  std::uniform_real_distribution<float> distributionZ(m_initialPos.m_z-m_emissionSourceSize.m_z/2.0f,m_initialPos.m_z+m_emissionSourceSize.m_z/2.0f);

  // Give all particles a new initial position based off that of the particle system.
  for (int i=0; i<m_particles.size(); i++)
  {
    m_particles[i].setInitialPosition(ngl::Vec3(distributionX(generator), distributionY(generator), distributionZ(generator)));
  }
}

void ParticleEmitter::setNewParticleInitialPosition()
{
  std::default_random_engine generator;
  generator.seed(time(0));

  std::uniform_real_distribution<float> distributionX(m_initialPos.m_x-m_emissionSourceSize.m_x/2.0f,m_initialPos.m_x+m_emissionSourceSize.m_x/2.0f);
  std::uniform_real_distribution<float> distributionY(m_initialPos.m_y-m_emissionSourceSize.m_y/2.0f,m_initialPos.m_y+m_emissionSourceSize.m_y/2.0f);
  std::uniform_real_distribution<float> distributionZ(m_initialPos.m_z-m_emissionSourceSize.m_z/2.0f,m_initialPos.m_z+m_emissionSourceSize.m_z/2.0f);

  // Give all particles a new initial position based off that of the particle system.
  for (int i=0; i<m_particles.size(); i++)
  {
    m_particles[i].setInitialPosition(ngl::Vec3(distributionX(generator), distributionY(generator), distributionZ(generator)));
  }
}


void ParticleEmitter::setGravity(ngl::Vec3 _gravity)
{
  m_gravity = _gravity;
}

void ParticleEmitter::setEmissionRate(float _emissionRate)
{
  m_emissionRate = _emissionRate;
  if (m_emissionRate > m_maxParticles)
  {
    m_emissionRate = m_maxParticles;
  }
}

void ParticleEmitter::setTotalParticles(int _numParticles)
{

}

void ParticleEmitter::setParticleMass(float _mass)
{
  for (unsigned long i=0; i<m_particles.size(); i++)
  {
    m_particles[i].m_mass = _mass;
  }
}

void ParticleEmitter::setParticleLifetime(float _lifetime)
{
  for (unsigned long i=0; i<m_particles.size(); i++)
  {
    m_particles[i].m_initialLifeTime = _lifetime;
  }
}

void ParticleEmitter::setDragCoefficient(float _drag)
{
  for (unsigned long i=0; i<m_particles.size(); i++)
  {
    m_particles[i].m_dragCoefficient = _drag;
  }
}

void ParticleEmitter::setEmissionSourceSize(ngl::Vec3 _size)
{
  m_emissionSourceSize = _size;

  std::default_random_engine generator;
  std::uniform_real_distribution<float> distributionX(m_initialPos.m_x-m_emissionSourceSize.m_x/2.0f,m_initialPos.m_x+m_emissionSourceSize.m_x/2.0f);
  std::uniform_real_distribution<float> distributionY(m_initialPos.m_y -m_emissionSourceSize.m_y/2.0f,m_initialPos.m_y + m_emissionSourceSize.m_y/2.0f);
  std::uniform_real_distribution<float> distributionZ(m_initialPos.m_z-m_emissionSourceSize.m_z/2.0f,m_initialPos.m_z+m_emissionSourceSize.m_z/2.0f);

  for (unsigned long i=0; i<m_particles.size(); i++)
  {
    m_particles[i].m_initialPos = ngl::Vec3(distributionX(generator), distributionY(generator), distributionZ(generator));
  }
}

void ParticleEmitter::setShaderName(char *_shaderName)
{
  m_shaderName = _shaderName;
}

void ParticleEmitter::emitParticles()
{
  for (int i=0; i<m_emissionRate; i++)
  {
    if (m_firstAvailble != NULL)
    {
      /// @brief The following section is from :-
      /// Robert Nystrom (2 Nov 2014). Game Programming Patterns [online]
      /// [Accessed 2015[. Available from: <http://gameprogrammingpatterns.com/object-pool.html>.
      Particle *p = m_firstAvailble;
      m_firstAvailble = p->getNext();
      /// end of citation

      p->reset();
    }
  }
}

void ParticleEmitter::update(float _dt)
{
  //emitParticles();

  for (int i=0; i<m_maxParticles; i++)
  {
    if (m_particles[i].m_active)
    {
      if (!m_particles[i].update(_dt, m_gravity))
      {
        /// @brief The following section is from :-
        /// Robert Nystrom (2 Nov 2014). Game Programming Patterns [online]
        /// [Accessed 2015[. Available from: <http://gameprogrammingpatterns.com/object-pool.html>.
        m_particles[i].setNext(m_firstAvailble);
        m_firstAvailble = &m_particles[i];
        /// end of citation
      }
    }
  }
}

void ParticleEmitter::draw(ngl::Mat4 _VP, ngl::Mat4 _V)
{
  glDisable( GL_DEPTH_TEST );
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  struct data
  {
    ngl::Vec3 pos;
    ngl::Vec3 color;
    ngl::Vec3 vel;
  };

  std::vector <data> particles;

  for(int i=0; i<m_maxParticles; ++i)
  {
    if (m_particles[i].m_active)
    {
      data p;
      p.pos = m_particles[i].m_pos;
      p.color = ngl::Vec3(m_particles[i].m_pressure, m_particles[i].m_density, 1);
      p.vel = ngl::Vec3(0,1,0);
      particles.push_back(p);
    }
  }

  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  (*shader)[m_shaderName]->use();

  shader->setRegisteredUniform("V",_V);
  shader->setShaderParamFromMat4("VP",_VP);
  ngl::VertexArrayObject *vao=ngl::VertexArrayObject::createVOA(GL_POINTS);
  vao->bind();
  vao->setData(particles.size()*sizeof(data),particles[0].pos.m_x,GL_DYNAMIC_DRAW);
  vao->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(data),0);
  vao->setVertexAttributePointer(1,3,GL_FLOAT,sizeof(data),3);
  vao->setVertexAttributePointer(2,3,GL_FLOAT,sizeof(data),6);
  vao->setNumIndices(particles.size());
  vao->draw();
  vao->unbind();
  vao->removeVOA();
  delete vao;
}
