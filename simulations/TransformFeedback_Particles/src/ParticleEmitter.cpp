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
  std::uniform_real_distribution<float> velRandX(-4, 4);
  std::uniform_real_distribution<float> velRand(4, 10);

  for (int i=0; i<m_maxParticles; i++)
  {
    Particle p;
    p.position.x = distributionX(generator);
    p.position.y = distributionY(generator);
    p.position.z = distributionZ(generator);
    p.position.w = 1;

    p.initialPosition = p.position;

    p.velocity.x = velRandX(generator);
    p.velocity.y = 0;
    p.velocity.z = velRand(generator);
    p.velocity.w = velRand(generator);

    p.lifetime.x = lifeTimeRand(generator);//The x value of lifetime is the time left for the particle
    p.lifetime.y = p.lifetime.x; //The y value is the initial lifetime, as in what the x value will be reset to when it falls below 0

    m_particles.push_back(p);
  }

  const GLchar* feedbackVaryings[4] = { "outPosition", "outInitialPosition", "outVelocity", "outLifetime" };
  m_feedback.initialize("shaders/particleAdvection.glsl",4,feedbackVaryings);
  m_feedback.setData(m_particles[0].position.x,m_maxParticles,m_maxParticles*sizeof(Particle));
  m_feedback.addVertexAttribute(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle),0);
  m_feedback.addVertexAttribute(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle),4);
  m_feedback.addVertexAttribute(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle),8);
  m_feedback.addVertexAttribute(3, 2, GL_FLOAT, GL_FALSE, sizeof(Particle),12);
}


void ParticleEmitter::update()
{
  m_feedback.useProgram();
  m_feedback.feedback();
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

  glBindBuffer(GL_ARRAY_BUFFER,m_feedback.getVBO());
  glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,sizeof(Particle),(GLvoid*)0);
  glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(Particle),(GLvoid*)(sizeof(GLfloat)*8));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glDrawArrays(GL_POINTS, 0, m_maxParticles);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vao);
}

