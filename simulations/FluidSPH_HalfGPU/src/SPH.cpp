#include "SPH.h"
#include "ngl/Vec2.h"
#include <iostream>
#include "math.h"
#include "ngl/ShaderLib.h"

// /home/i7657309/masters/simulations/FluidSPH_GPU/FluidSPH_GPU

//Constructor setting default values
SPH::SPH(float _dt)
{
  m_linearViscosity = 0.0005f;
  m_quadraticViscosity = 0.0005f;
  m_kernel = 1.2f;
  m_stiffness = 1.16f;
  m_nearStiffness = 1.01f;
  m_restDensity = 10.80f;
  m_particleRadius = m_kernel*1.0f;
  m_dt = _dt;
  m_numParticles = 256*40; //256 is num work groups

  initialize();
}

SPH::~SPH()
{

}

void SPH::initialize()
{
  m_2d = 0;

  m_wind = ngl::Vec3(0,0,0);
  m_gravity = ngl::Vec3(0,-90.8f,0);

  /////////////////        Particle Emitter       //////////////////////////////

  float emitSize;
  if (m_2d)
  {
    emitSize = m_numParticles * 0.004f;
  }
  else
  {
    emitSize = m_numParticles * 0.001f;
  }

  m_particleEmitter.reset(new ParticleEmitter(m_numParticles,ngl::Vec3(emitSize,emitSize,emitSize), 10000000000, 5.0f, 0.2f, "Color"));

  m_particleEmitter->setEmissionRate(1000);
  m_particleEmitter->setGravity(ngl::Vec3(0,-10.0f,0));
  m_particleEmitter->setInitialPosition(ngl::Vec3(0,0,0));

  for (unsigned int i=0; i<m_particleEmitter->m_particles.size();i++)
  {
    m_particleEmitter->m_particles[i].m_density = 0;
    if (m_2d) m_particleEmitter->m_particles[i].m_pos.m_z = 0;
  }
  m_particleEmitter->emitParticles();

  /////////////////        Vector Field       /////////////////////////////////
  float scale = m_kernel;
  m_gridSize = 20;
  ngl::Vec3 fieldPosition = ngl::Vec3(0,0,0);
  int zRows = m_gridSize;
  if (m_2d)
  {
    zRows = 1;
  }
  m_vectorField.reset(new VectorField(fieldPosition,m_gridSize,m_gridSize,zRows,ngl::Vec3(scale,scale,scale)));
  m_vectorField->setVectorMagnitude(1.0f);

  m_numCells = m_vectorField->m_nodeList.size();

  /////////////////        Octree           /////////////////////////////////
  BoundingBox octreeBounds;
  float boxRadius = scale*m_gridSize / 2.0f;
  octreeBounds.m_minx = -boxRadius; octreeBounds.m_maxx = boxRadius;
  octreeBounds.m_miny = -boxRadius; octreeBounds.m_maxy = boxRadius;
  octreeBounds.m_minz = -boxRadius; octreeBounds.m_maxz = boxRadius;
  m_octree.reset(new Octree(4,fieldPosition,octreeBounds));
  for (unsigned long i=0; i<m_vectorField->m_nodeList.size(); i++)
  {
    m_octree->addVectorNode(&m_vectorField->m_nodeList[i]);
  }

  m_bounds = octreeBounds;

  addBoundaryDrawData(octreeBounds);
  initializeBoundsVAO();


  //////////////////////        Compute Shaders         /////////////////////////////
  int workGroups = (m_numParticles / 256);
  m_viscoCompute.initialize("shaders/comp_viscosity.glsl",workGroups);
  m_advectCompute.initialize("shaders/comp_advection.glsl",workGroups);
  m_densPressDispCompute.initialize("shaders/comp_densPressDisp.glsl",workGroups);
  m_collisionsCompute.initialize("shaders/comp_collision.glsl",workGroups);

  doGPU::SSBO ssbo;
  //Positions
  ssbo.createSSBO(m_numParticles*sizeof(vec4),0);
  struct vec4* positions = (struct vec4*)ssbo.mapDataToBuffer(m_numParticles*sizeof(vec4));
  for (unsigned int i=0; i<m_numParticles; i++)
  {
    positions[i].x = m_particleEmitter->m_particles[i].m_pos.m_x;
    positions[i].y = m_particleEmitter->m_particles[i].m_pos.m_y;
    positions[i].z = m_particleEmitter->m_particles[i].m_pos.m_z;
    if (m_2d){positions[i].z = 0;}
    positions[i].w = 1;
  }
  ssbo.unmapBuffer();
  m_ssbos["positionBuffer"] = ssbo;

  //Previous Positions
  ssbo.createSSBO(m_numParticles*sizeof(vec4),1);
  struct vec4* prevPositions = (struct vec4*)ssbo.mapDataToBuffer(m_numParticles*sizeof(vec4));
  for (unsigned int i=0; i<m_numParticles; i++)
  {
    prevPositions[i].x = m_particleEmitter->m_particles[i].m_pos.m_x;
    prevPositions[i].y = m_particleEmitter->m_particles[i].m_pos.m_y;
    prevPositions[i].z = m_particleEmitter->m_particles[i].m_pos.m_z;
    if (m_2d){prevPositions[i].z = 0;}
    prevPositions[i].w = 1;
  }
  ssbo.unmapBuffer();
  m_ssbos["prevPositionBuffer"] = ssbo;

  //Velocity
  ssbo.createSSBO(m_numParticles*sizeof(vec4),2);
  struct vec4* velocities = (struct vec4*)ssbo.mapDataToBuffer(m_numParticles*sizeof(vec4));
  for (unsigned int i=0; i<m_numParticles; i++)
  {
    velocities[i].x = m_particleEmitter->m_particles[i].m_velocity.m_x;
    velocities[i].y = m_particleEmitter->m_particles[i].m_velocity.m_y;
    velocities[i].z = m_particleEmitter->m_particles[i].m_velocity.m_z;
    velocities[i].w = 1;
  }
  ssbo.unmapBuffer();
  m_ssbos["velocityBuffer"] = ssbo;

  //Density
  ssbo.createSSBO(m_numParticles*sizeof(GLfloat),3);
  GLfloat* dens = (GLfloat*)ssbo.mapDataToBuffer(m_numParticles*sizeof(GLfloat));
  for (unsigned int i=0; i<m_numParticles; i++) { dens[i] = 0; }
  ssbo.unmapBuffer();
  m_ssbos["densityBuffer"] = ssbo;

  //Near Density
  ssbo.createSSBO(m_numParticles*sizeof(GLfloat),4);
  GLfloat* nearDens = (GLfloat*)ssbo.mapDataToBuffer(m_numParticles*sizeof(GLfloat));
  for (unsigned int i=0; i<m_numParticles; i++) { nearDens[i] = 0; }
  ssbo.unmapBuffer();
  m_ssbos["nearDensityBuffer"] = ssbo;

  //Pressure
  ssbo.createSSBO(m_numParticles*sizeof(GLfloat),5);
  GLfloat* pressure = (GLfloat*)ssbo.mapDataToBuffer(m_numParticles*sizeof(GLfloat));
  for (unsigned int i=0; i<m_numParticles; i++) { pressure[i] = 0; }
  ssbo.unmapBuffer();
  m_ssbos["pressureBuffer"] = ssbo;

  //Near Pressure
  ssbo.createSSBO(m_numParticles*sizeof(GLfloat),6);
  GLfloat* nearPressure = (GLfloat*)ssbo.mapDataToBuffer(m_numParticles*sizeof(GLfloat));
  for (unsigned int i=0; i<m_numParticles; i++) { nearPressure[i] = 0; }
  ssbo.unmapBuffer();
  m_ssbos["nearPressureBuffer"] = ssbo;

  //Neighbours
  ssbo.createSSBO(m_numParticles*sizeof(GLint)*MAX_NEIGHBOURS,7);
  GLint* neighbours = (GLint*)ssbo.mapDataToBuffer(m_numParticles*sizeof(GLint)*MAX_NEIGHBOURS);
  for (unsigned int i=0; i<m_numParticles; i++)
  {
    for (unsigned int j=0; j<MAX_NEIGHBOURS; j++)
    {
      neighbours[i*MAX_NEIGHBOURS+j] = -1;
    }
  }
  ssbo.unmapBuffer();
  m_ssbos["neighboursBuffer"] = ssbo;

  addParticlesToCells();
  fillParticleNeighbours();
}


void SPH::addParticlesToCells()
{
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbos["positionBuffer"].getBufferID());
  GLfloat* positions = (GLfloat*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

  //Add particles to cells
  m_octree->clearParticlesFromTreeNodeAndVectorNodes(m_octree->m_rootNode);

  for (unsigned int i =0; i<m_numParticles; i++)
  {
    m_particleEmitter->m_particles[i].m_pos.m_x = positions[(i*4)];
    m_particleEmitter->m_particles[i].m_pos.m_y = positions[(i*4)+1];
    m_particleEmitter->m_particles[i].m_pos.m_z = positions[(i*4)+2];

    //std::cout<<positions[i*4]<< ", " << positions[(i*4)+1] << ", " << positions[(i*4)+2] << "\n";

    if (m_particleEmitter->m_particles[i].m_active)
    {
      m_octree->addParticleToTreeAndVectorNodes(&m_particleEmitter->m_particles[i]);
    }
  }

  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void SPH::fillParticleNeighbours()
{
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbos["neighboursBuffer"].getBufferID());
  GLint* neighbours = (GLint*)m_ssbos["neighboursBuffer"].mapDataToBuffer(m_numParticles*sizeof(GLint)*MAX_NEIGHBOURS);
  //Run through every cell and each particle in that cell.

  for (unsigned int i=0; i<m_numParticles; i++)
  {
    for (unsigned int j=0; j<MAX_NEIGHBOURS; j++)
    {
      neighbours[i*MAX_NEIGHBOURS + j] = -1;
    }
  }

  int num = 0;
  for (unsigned int i=0; i<m_numCells; i++)
  {
    for (unsigned int j=0;j<m_vectorField->m_nodeList[i].m_particleList.size(); j++)
    {
      Particle *p  = m_vectorField->m_nodeList[i].m_particleList[j];
      p->m_neighbours.clear();
      for (unsigned int nS=0; nS<MAX_NEIGHBOURS; nS++)
      {
        neighbours[p->m_index*MAX_NEIGHBOURS + nS] = -1;
      }
      unsigned int nCount=0;
      num++;
      //Go through all surrounding and current cells of the current particle, calculating density and pressure based on other particles in those cells
      for (int k=-1; k<2; k++)
      {
        for (int l=-1; l<2; l++)
        {
          unsigned int index = (i+k) + (l*m_gridSize);

          if (index >= m_numCells)
          {
            continue;
          }

          for (unsigned int m=0; m<m_vectorField->m_nodeList[index].m_particleList.size(); m++)
          {
            Particle* otherP = m_vectorField->m_nodeList[index].m_particleList[m];
            ngl::Vec3 dir = otherP->m_pos - p->m_pos;
            float distSquared = dir.lengthSquared();

            if ((distSquared > m_kernel*m_kernel || distSquared < 0.0000001f))
            {
              continue;
            }

            p->m_neighbours.push_back(otherP);
            neighbours[p->m_index*MAX_NEIGHBOURS + nCount] = otherP->m_index;

            nCount++;

            if (nCount >= MAX_NEIGHBOURS)
            {
              k = 2;
              l = 2;
              break;
            }
          }
        }
      }
    }
  }
  m_ssbos["neighboursBuffer"].unmapBuffer();
}


void SPH::update()
{
  ///GPU
  for (auto buffer : m_ssbos)
  {
    buffer.second.bindBufferToLocation();
  }

  m_viscoCompute.useProgram();
  glUniform1f(glGetUniformLocation(m_viscoCompute.getComputePrgram(), "dt") , m_dt);
  glUniform1f(glGetUniformLocation(m_viscoCompute.getComputePrgram(), "kernel") , m_kernel);
  glUniform1f(glGetUniformLocation(m_viscoCompute.getComputePrgram(), "linearViscosity") , m_linearViscosity);
  glUniform1f(glGetUniformLocation(m_viscoCompute.getComputePrgram(), "quadraticViscosity") , m_quadraticViscosity);
  m_viscoCompute.dispatch();

  m_advectCompute.useProgram();
  glUniform1f(glGetUniformLocation(m_advectCompute.getComputePrgram(), "dt") , m_dt);
  glUniform1f(glGetUniformLocation(m_advectCompute.getComputePrgram(), "gravity") , m_gravity.m_y);
  m_advectCompute.dispatch();

  addParticlesToCells();
  fillParticleNeighbours();

  m_densPressDispCompute.useProgram();
  glUniform1f(glGetUniformLocation(m_densPressDispCompute.getComputePrgram(), "dt") , m_dt);
  glUniform1f(glGetUniformLocation(m_densPressDispCompute.getComputePrgram(), "kernel") , m_kernel);
  glUniform1f(glGetUniformLocation(m_densPressDispCompute.getComputePrgram(), "restDensity") , m_restDensity);
  glUniform1f(glGetUniformLocation(m_densPressDispCompute.getComputePrgram(), "stiffness") , m_stiffness);
  glUniform1f(glGetUniformLocation(m_densPressDispCompute.getComputePrgram(), "nearStiffness") , m_nearStiffness);
  m_densPressDispCompute.dispatch();


  m_collisionsCompute.useProgram();
  glUniform1f(glGetUniformLocation(m_collisionsCompute.getComputePrgram(), "dt") , m_dt);
  glUniform1f(glGetUniformLocation(m_collisionsCompute.getComputePrgram(), "particleRadius") , m_particleRadius);
  glUniform1f(glGetUniformLocation(m_collisionsCompute.getComputePrgram(), "halfBoundsSize") , m_bounds.m_maxx);
  glUniform3f(glGetUniformLocation(m_collisionsCompute.getComputePrgram(), "boundsCenter"),0,0,0);
  m_collisionsCompute.dispatch();


}

void SPH::draw(ngl::Mat4 _VP, ngl::Vec3 _cameraPosition, GLuint _particleTextureID)
{
  glBindTexture(GL_TEXTURE_2D,_particleTextureID);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //m_particleEmitter->draw(_VP, _cameraPosition);

  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  (*shader)["Color"]->use();

  shader->setShaderParamFromMat4("VP",_VP);
  shader->setRegisteredUniform("cameraPosition",_cameraPosition);

  m_vao->bind();
  m_vao->draw();
  m_vao->unbind();

  GLuint vao;
  glGenVertexArrays(1,&vao);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, m_ssbos["positionBuffer"].getBufferID());
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vec4),(GLvoid*)0);
  glBindBuffer(GL_ARRAY_BUFFER, m_ssbos["pressureBuffer"].getBufferID());
  glVertexAttribPointer(1,1,GL_FLOAT,GL_FALSE,sizeof(GLfloat),(GLvoid*)0);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glDrawArrays(GL_POINTS, 0, m_numParticles);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vao);
}

void SPH::initializeBoundsVAO()
{
  m_vao=ngl::VertexArrayObject::createVOA(GL_LINES);
  m_vao->bind();
  m_vao->setData(m_drawData.size()*sizeof(ngl::Vec3),m_drawData[0].m_x);
  m_vao->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(ngl::Vec3),0);
  m_vao->setNumIndices(m_drawData.size());

  m_vao->unbind();
}

void SPH::addBoundaryDrawData(BoundingBox _bounds)
{
  ngl::Vec3 v1 = ngl::Vec3(_bounds.m_minx,_bounds.m_miny,_bounds.m_minz);
  ngl::Vec3 v2 = ngl::Vec3(_bounds.m_minx,_bounds.m_maxy,_bounds.m_minz);
  ngl::Vec3 v3 = ngl::Vec3(_bounds.m_minx,_bounds.m_miny,_bounds.m_maxz);
  ngl::Vec3 v4 = ngl::Vec3(_bounds.m_minx,_bounds.m_maxy,_bounds.m_maxz);

  ngl::Vec3 v5 = ngl::Vec3(_bounds.m_maxx,_bounds.m_miny,_bounds.m_minz);
  ngl::Vec3 v6 = ngl::Vec3(_bounds.m_maxx,_bounds.m_maxy,_bounds.m_minz);
  ngl::Vec3 v7 = ngl::Vec3(_bounds.m_maxx,_bounds.m_miny,_bounds.m_maxz);
  ngl::Vec3 v8 = ngl::Vec3(_bounds.m_maxx,_bounds.m_maxy,_bounds.m_maxz);

  //Drawing cube using lines (involves duplicate points)
  m_drawData.push_back(v1);m_drawData.push_back(v2); //1 -> 2
  m_drawData.push_back(v2);m_drawData.push_back(v4); //2 -> 4 ...
  m_drawData.push_back(v4);m_drawData.push_back(v3);
  m_drawData.push_back(v3);m_drawData.push_back(v1);

  m_drawData.push_back(v1);m_drawData.push_back(v5);
  m_drawData.push_back(v5);m_drawData.push_back(v7);
  m_drawData.push_back(v7);m_drawData.push_back(v3);

  m_drawData.push_back(v4);m_drawData.push_back(v8);
  m_drawData.push_back(v8);m_drawData.push_back(v6);
  m_drawData.push_back(v6);m_drawData.push_back(v2);

  m_drawData.push_back(v8);m_drawData.push_back(v7);

  m_drawData.push_back(v6);m_drawData.push_back(v5);
}

void SPH::reset()
{
  m_particleEmitter->setNewParticleInitialPosition();

  for (unsigned int i =0; i<m_numParticles; i++)
  {
    m_particleEmitter->m_particles[i].reset();
  }
}

void SPH::setWindX(float _x)
{
  m_wind = ngl::Vec3(_x,m_wind.m_y,m_wind.m_z);
}

void SPH::setWindY(float _y)
{
  m_wind = ngl::Vec3(m_wind.m_x,_y,m_wind.m_z);
}

void SPH::setKernel(float _v)
{
  m_kernel = _v;
}

void SPH::setRestDensity(float _v)
{
  m_restDensity = _v;
}

void SPH::setLinViscosity(float _v)
{
  m_linearViscosity = _v;
}

void SPH::setQuadViscosity(float _v)
{
  m_quadraticViscosity = _v;
}

void SPH::setStiffness(float _v)
{
  m_stiffness = _v;
}

void SPH::setNearStiffness(float _v)
{
  m_nearStiffness = _v;
}

void SPH::setGravity(float _v)
{
  m_gravity = ngl::Vec3(0,_v,0);
}
