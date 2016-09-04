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
#include <fstream>

ParticleEmitter::ParticleEmitter()
{

}

ParticleEmitter::~ParticleEmitter()
{

}

void ParticleEmitter::initialize()
{
  std::default_random_engine generator;
  generator.seed(time(0));

  m_maxParticles = 2000000;
  m_particles.reserve(sizeof(Particle)*m_maxParticles);

  std::uniform_real_distribution<float> distributionX(-20,20);
  std::uniform_real_distribution<float> distributionY(-10,5);
  std::uniform_real_distribution<float> distributionZ(5,40);

  std::uniform_real_distribution<float> lifeTimeRand(40, 60);
  std::uniform_real_distribution<float> velRand(4, 10);

  //Set up compute shader and SSBOs
  m_compute.initialize("shaders/particleCompute.glsl", (m_maxParticles / 256)+1);

  struct vec4* positions;
  m_compute.createSSBO("PositionBuffer",m_maxParticles * sizeof(struct vec4),4);
  positions = (struct vec4*)m_compute.mapDataToBuffer("PositionBuffer",m_maxParticles * sizeof(struct vec4));
  for (int i=0; i<m_maxParticles; i++)
  {
    positions[i].x = distributionX(generator);
    positions[i].y = distributionY(generator);
    positions[i].z = distributionZ(generator);
    positions[i].w = 1;
  }
  m_compute.unmapBuffer();

  struct vec4* initialPositions;
  m_compute.createSSBO("InitialPositionBuffer",m_maxParticles * sizeof(struct vec4),5);
  initialPositions = (struct vec4*)m_compute.mapDataToBuffer("InitialPositionBuffer",m_maxParticles * sizeof(struct vec4));

  for (int i=0; i<m_maxParticles; i++)
  {
    initialPositions[i] = positions[i];
  }
  m_compute.unmapBuffer();

  struct vec4* velocities;
  m_compute.createSSBO("VelocityBuffer",m_maxParticles * sizeof(struct vec4),6);
  velocities = (struct vec4*)m_compute.mapDataToBuffer("VelocityBuffer",m_maxParticles * sizeof(struct vec4));
  for (int i=0; i<m_maxParticles; i++)
  {
    velocities[i].x = velRand(generator);
    velocities[i].y = 0;
    velocities[i].z = velRand(generator);
    velocities[i].w = velRand(generator);
  }
  m_compute.unmapBuffer();

  vec4* lifetimes;
  m_compute.createSSBO("LifeTimeBuffer",m_maxParticles * sizeof(vec4),7);
  lifetimes = (vec4*)m_compute.mapDataToBuffer("LifeTimeBuffer",m_maxParticles * sizeof(vec4));
  for (int i=0; i<m_maxParticles; i++)
  {
    lifetimes[i].x = lifeTimeRand(generator);
    lifetimes[i].y = lifetimes[i].x; //will be kept as the initial lifetime
    lifetimes[i].z = 1;
    lifetimes[i].w = 1;
  }
  m_compute.unmapBuffer();
}


void ParticleEmitter::update()
{
  m_compute.useProgram();
  m_compute.dispatch();
}

void ParticleEmitter::draw(ngl::Mat4 _VP, ngl::Vec3 _cameraPosition)
{
  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  (*shader)["Color"]->use();

  shader->setRegisteredUniform("cameraPosition",_cameraPosition);
  shader->setShaderParamFromMat4("VP",_VP);

  GLuint vao;
  glGenVertexArrays(1,&vao);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, m_compute.getSSBO("PositionBuffer"));
  glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,sizeof(vec4),(GLvoid*)0);
  glBindBuffer(GL_ARRAY_BUFFER, m_compute.getSSBO("VelocityBuffer"));
  glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(vec4),(GLvoid*)0);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glDrawArrays(GL_POINTS, 0, m_maxParticles);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vao);
}

