#include "SPH.h"
#include "ngl/Vec2.h"
#include <iostream>
#include "math.h"
#include "ngl/Util.h"
#include "ngl/ShaderLib.h"

// /home/i7657309/masters/simulations/FluidSPH_GPU/FluidSPH_GPU

const int MAX_NEIGHBOURS = 20;
const int MAX_PARTICLES_PER_CELL = 20;
const int NUM_CELLS = 16;

//Constructor setting default values
SPH::SPH(float _dt)
{
  m_linearViscosity = 2.0005f;
  m_quadraticViscosity = 2.0005f;
  m_kernel = 1.0f;
  m_stiffness = 10.16f;
  m_nearStiffness = 10.11f;
  m_restDensity = 6.80f;
  m_particleRadius = m_kernel*1.0f;
  m_dt = _dt;
  m_numParticles = 16; //256 is num work groups

  initialize();
}

SPH::~SPH()
{

}

void SPH::initialize()
{
  m_2d = 0;

  m_gravity = ngl::Vec3(0,-9.8f,0);

  /////////////////        Particle Emitter       //////////////////////////////

  float emitSize;
  if (m_2d)
  {
    emitSize = m_numParticles * 0.01f;
  }
  else
  {
    emitSize = m_numParticles * 0.001f;
  }

  std::unique_ptr<ParticleEmitter> particleEmitter;
  particleEmitter.reset(new ParticleEmitter(m_numParticles,ngl::Vec3(emitSize,emitSize,emitSize), 10000000000, 5.0f, 0.2f, "Color"));

  particleEmitter->setEmissionRate(1000);
  particleEmitter->setGravity(ngl::Vec3(0,-20.0f,0));
  particleEmitter->setInitialPosition(ngl::Vec3(0,0,0));

  for (unsigned int i=0; i<particleEmitter->m_particles.size();i++)
  {
    particleEmitter->m_particles[i].m_density = 0;
    if (m_2d) particleEmitter->m_particles[i].m_pos.m_z = 0;
  }

  //////////////////////        GPU Grid Setup        /////////////////////////////
  m_gridCenter = ngl::Vec3(0,0,0);
  unsigned int numZCells = NUM_CELLS;
  if (m_2d) numZCells = 1;

  //Grid offset is used to move the grid so the lower left back corner is at 0,0,0 when calculating which cells particles are in.
  //(m_kernel/2.0f * NUM_CELLS) is half the grid size.
  float offsetX = m_gridCenter.m_x - (m_kernel/2.0f * NUM_CELLS);
  float offsetY = m_gridCenter.m_y - (m_kernel/2.0f * NUM_CELLS);
  float offsetZ = m_gridCenter.m_z - (m_kernel/2.0f * numZCells);

  if (offsetX < 0.0f){ m_gridOffset.m_x = fabs(offsetX); }
  else{ m_gridOffset.m_x = -offsetX; }

  if (offsetY < 0.0f){ m_gridOffset.m_y = fabs(offsetY); }
  else{ m_gridOffset.m_y = -offsetY; }

  if (offsetZ < 0.0f){ m_gridOffset.m_z = fabs(offsetZ); }
  else{ m_gridOffset.m_z = -offsetZ; }

  initializeBoundsVAO();


  //////////////////////        Compute Shaders         /////////////////////////////
  m_viscoCompute.initialize("shaders/comp_viscosity.glsl",(NUM_CELLS*NUM_CELLS*numZCells)/256); //Instead of being run once per particle, this is run once per cell of the grid so as to find the neighbours of each particle
  m_advectCompute.initialize("shaders/comp_advection.glsl",m_numParticles / 256);
  m_clearGridCompute.initialize("shaders/comp_clearGrid.glsl", ((MAX_PARTICLES_PER_CELL+4)*NUM_CELLS*NUM_CELLS*numZCells)/256);
  m_addToGridCompute.initialize("shaders/comp_addToGrid.glsl",m_numParticles / 16);
  m_densPressDispCompute.initialize("shaders/comp_densPressDisp.glsl",m_numParticles / 256);
  m_collisionsCompute.initialize("shaders/comp_collision.glsl",m_numParticles / 256);
  m_testCompute.initialize("shaders/comp_test.glsl",1);


  //////////////////////        SSBOs         /////////////////////////////
  doGPU::SSBO ssbo;
  //Positions
  ssbo.createSSBO(m_numParticles*sizeof(vec4),0);
  struct vec4* positions = (struct vec4*)ssbo.mapDataToBuffer(m_numParticles*sizeof(vec4));
  for (unsigned int i=0; i<m_numParticles; i++)
  {
    positions[i].x = particleEmitter->m_particles[i].m_pos.m_x;
    positions[i].y = particleEmitter->m_particles[i].m_pos.m_y;
    positions[i].z = particleEmitter->m_particles[i].m_pos.m_z;
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
    prevPositions[i].x = particleEmitter->m_particles[i].m_pos.m_x;
    prevPositions[i].y = particleEmitter->m_particles[i].m_pos.m_y;
    prevPositions[i].z = particleEmitter->m_particles[i].m_pos.m_z;
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
    velocities[i].x = particleEmitter->m_particles[i].m_velocity.m_x;
    velocities[i].y = particleEmitter->m_particles[i].m_velocity.m_y;
    velocities[i].z = particleEmitter->m_particles[i].m_velocity.m_z;
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

  //Grid
  //The +4 is to take into account the first 4 ints of each cell in this buffer store number of particles
  //and the position of the cell in the grid, as in, the x,y and z. The rest of the space per cell is taken
  //up by the indices of particles in their respective buffers.
  int numElements = (MAX_PARTICLES_PER_CELL + 4)*NUM_CELLS*NUM_CELLS*numZCells;
  ssbo.createSSBO(sizeof(GLint)*numElements,7);
  GLint* grid = (GLint*)ssbo.mapDataToBuffer(sizeof(GLint)*numElements);
  for (unsigned int i=0; i<numElements; i++)
  {
    grid[i] = 0;
  }
  ssbo.unmapBuffer();
  m_ssbos["gridBuffer"] = ssbo;

  //Neighbours
  ssbo.createSSBO(m_numParticles*sizeof(GLint)*(MAX_NEIGHBOURS+1),8);
  GLint* neighbours = (GLint*)ssbo.mapDataToBuffer(m_numParticles*sizeof(GLint)*(MAX_NEIGHBOURS+1)); //+1 as the first element stores the number of neighbour particles
  for (unsigned int i=0; i<m_numParticles*(MAX_NEIGHBOURS+1); i++)
  {
    neighbours[i] = -1;
  }
  ssbo.unmapBuffer();
  m_ssbos["neighbourBuffer"] = ssbo;

  ssbo.createSSBO(m_numParticles*sizeof(GLint),8);
  GLint* debugInt = (GLint*)ssbo.mapDataToBuffer(m_numParticles*sizeof(GLint));
  for (unsigned int i=0; i<m_numParticles*(MAX_NEIGHBOURS+1); i++)
  {
    debugInt[i] = 0.0f;
  }
  ssbo.unmapBuffer();
  m_ssbos["intDebugBuffer"] = ssbo;

  ssbo.createSSBO(16*sizeof(GLfloat),9);
  GLfloat* debugFloat = (GLfloat*)ssbo.mapDataToBuffer(16*sizeof(GLfloat));
  for (unsigned int i=0; i<16; i++)
  {
    debugFloat[i] = i;
  }
  ssbo.unmapBuffer();
  m_ssbos["floatDebugBuffer"] = ssbo;

  for (auto buffer : m_ssbos)
  {
    buffer.second.bindBufferToLocation();
  }

  //Set up the grid for the viscosity shader
  m_clearGridCompute.useProgram();
  glUniform1i(glGetUniformLocation(m_clearGridCompute.getComputePrgram(), "maxParticlesPerCell") , MAX_PARTICLES_PER_CELL);
  m_clearGridCompute.dispatch();

  m_addToGridCompute.useProgram();
  glUniform1i(glGetUniformLocation(m_addToGridCompute.getComputePrgram(), "numCellsPerAxis") , NUM_CELLS);
  glUniform1f(glGetUniformLocation(m_addToGridCompute.getComputePrgram(), "cellSize") , m_kernel);
  glUniform3f(glGetUniformLocation(m_addToGridCompute.getComputePrgram(), "gridOffset"),m_gridOffset.m_x,m_gridOffset.m_y,m_gridOffset.m_z);
  glUniform1i(glGetUniformLocation(m_addToGridCompute.getComputePrgram(), "maxParticlesPerCell") , MAX_PARTICLES_PER_CELL);
  m_addToGridCompute.dispatch();

  //std::cout<<"Implementation not working! Update not called on fluid.\n";



  m_ssbos["floatDebugBuffer"].bindBufferToLocation();

  m_testCompute.useProgram();
  m_testCompute.dispatch();

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbos["floatDebugBuffer"].getBufferID());
  GLfloat* debugPrint = (GLfloat*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  for (int i=0; i<16; i++)
  {
    std::cout<< "floatDebugBuffer["<< i << "]: " << debugPrint[i] << "\n";
  }

}


void SPH::update()
{

  ///GPU
  for (auto buffer : m_ssbos)
  {
    buffer.second.bindBufferToLocation();
  }

  //Viscosity
  m_viscoCompute.useProgram();
  glUniform1f(glGetUniformLocation(m_viscoCompute.getComputePrgram(), "dt") , m_dt);
  glUniform1f(glGetUniformLocation(m_viscoCompute.getComputePrgram(), "kernel") , m_kernel);
  glUniform1f(glGetUniformLocation(m_viscoCompute.getComputePrgram(), "linearViscosity") , m_linearViscosity);
  glUniform1f(glGetUniformLocation(m_viscoCompute.getComputePrgram(), "quadraticViscosity") , m_quadraticViscosity);
  glUniform1i(glGetUniformLocation(m_viscoCompute.getComputePrgram(), "numCellsPerAxis") , NUM_CELLS);
  glUniform1i(glGetUniformLocation(m_viscoCompute.getComputePrgram(), "maxNeighbours") , MAX_NEIGHBOURS);
  glUniform1i(glGetUniformLocation(m_viscoCompute.getComputePrgram(), "maxParticlesPerCell") , MAX_PARTICLES_PER_CELL);
  m_viscoCompute.dispatch();

  //Advection
  m_advectCompute.useProgram();
  glUniform1f(glGetUniformLocation(m_advectCompute.getComputePrgram(), "dt") , m_dt);
  glUniform1f(glGetUniformLocation(m_advectCompute.getComputePrgram(), "gravity") , m_gravity.m_y);
  m_advectCompute.dispatch();

  //Clear the grid of particles
  m_clearGridCompute.useProgram();
  glUniform1i(glGetUniformLocation(m_clearGridCompute.getComputePrgram(), "maxParticlesPerCell") , MAX_PARTICLES_PER_CELL);
  m_clearGridCompute.dispatch();

  //Add particles to cells
  m_addToGridCompute.useProgram();
  glUniform1i(glGetUniformLocation(m_addToGridCompute.getComputePrgram(), "numCellsPerAxis") , NUM_CELLS);
  glUniform1f(glGetUniformLocation(m_addToGridCompute.getComputePrgram(), "cellSize") , m_kernel);
  glUniform3f(glGetUniformLocation(m_addToGridCompute.getComputePrgram(), "gridOffset"),m_gridOffset.m_x,m_gridOffset.m_y,m_gridOffset.m_z);
  glUniform1i(glGetUniformLocation(m_addToGridCompute.getComputePrgram(), "maxParticlesPerCell") , MAX_PARTICLES_PER_CELL);
  m_addToGridCompute.dispatch();

  //Density Pressure and Displacement
  m_densPressDispCompute.useProgram();
  glUniform1f(glGetUniformLocation(m_densPressDispCompute.getComputePrgram(), "dt") , m_dt);
  glUniform1f(glGetUniformLocation(m_densPressDispCompute.getComputePrgram(), "kernel") , m_kernel);
  glUniform1f(glGetUniformLocation(m_densPressDispCompute.getComputePrgram(), "restDensity") , m_restDensity);
  glUniform1f(glGetUniformLocation(m_densPressDispCompute.getComputePrgram(), "stiffness") , m_stiffness);
  glUniform1f(glGetUniformLocation(m_densPressDispCompute.getComputePrgram(), "nearStiffness") , m_nearStiffness);
  glUniform1i(glGetUniformLocation(m_densPressDispCompute.getComputePrgram(), "maxNeighbours") , MAX_NEIGHBOURS);
  m_densPressDispCompute.dispatch();

  //Collisions
  m_collisionsCompute.useProgram();
  glUniform1f(glGetUniformLocation(m_collisionsCompute.getComputePrgram(), "dt") , m_dt);
  glUniform1f(glGetUniformLocation(m_collisionsCompute.getComputePrgram(), "particleRadius") , m_particleRadius);
  glUniform1f(glGetUniformLocation(m_collisionsCompute.getComputePrgram(), "halfBoundsSize") ,m_kernel/2.0f * NUM_CELLS);
  glUniform3f(glGetUniformLocation(m_collisionsCompute.getComputePrgram(), "boundsCenter"),m_gridCenter.m_x,m_gridCenter.m_y,m_gridCenter.m_z);
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
  glBindBuffer(GL_ARRAY_BUFFER, m_ssbos["densityBuffer"].getBufferID());
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
