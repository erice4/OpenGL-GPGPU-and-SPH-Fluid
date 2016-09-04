#include "SPH.h"
#include "ngl/Vec2.h"
#include <iostream>
#include "math.h"
#include "ngl/ShaderLib.h"
#include "ngl/Mat4.h"
#include "ngl/Transformation.h"
#include "ObstacleScene.h"

//Constructor setting default values
SPH::SPH(float _dt)
{
  m_linearViscosity = 0.41f;
  m_quadraticViscosity = 1.02f;
  m_kernel = 1.0f;
  m_stiffness = 4.6f;
  m_nearStiffness = 3.001f;
  m_restDensity = 6.80f;
  m_particleRadius = m_kernel*0.5f;
  m_dt = _dt;
  m_numParticles = 1500;
  m_bounce = 0.6f;

  m_2d = 1;

  m_gridSize = 20;

  m_gridZSize = m_gridSize;
  if (m_2d)
  {
    m_gridZSize = 1;
  }

  initialize();
}

SPH::~SPH()
{

}

void SPH::initialize()
{
  m_wind = ngl::Vec3(0,0,0);
  m_gravity = ngl::Vec3(0,-9.8f,0);

  /////////////////        Particle Emitter       //////////////////////////////

  float emitSize;
  if (m_2d)
  {
    emitSize = m_numParticles * 0.005f;
  }
  else
  {
    emitSize = m_numParticles * 0.002f;
  }

  m_particleEmitter.reset(new ParticleEmitter(m_numParticles,ngl::Vec3(emitSize,emitSize,emitSize), 10000000000, 5.0f, 0.2f, "Particle"));

  m_particleEmitter->setEmissionRate(3000);
  m_particleEmitter->setInitialPosition(ngl::Vec3(0,0,0));

  for (unsigned int i=0; i<m_particleEmitter->m_particles.size();i++)
  {
    m_particleEmitter->m_particles[i].m_density = 0;
  }


  /////////////////        Vector Field       /////////////////////////////////
  float scale = m_kernel;

  ngl::Vec3 fieldPosition = ngl::Vec3(0,0,0);

  m_vectorField.reset(new VectorField(fieldPosition,m_gridSize,m_gridSize,m_gridZSize,ngl::Vec3(scale,scale,scale)));
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
  m_distEdgeToCentreSquared = ngl::Vec3(m_bounds.m_maxx - fieldPosition.m_x, m_bounds.m_maxy - fieldPosition.m_y, m_bounds.m_maxz - fieldPosition.m_z).lengthSquared();

  addBoundaryDrawData(m_staticDrawData, octreeBounds);

  addParticlesToCells();

  //Just using this method to fill the particle's neighbours
  if (!m_2d)
  {
    calculateDensityPressureDisplacement();
  }
  else
  {
    calculateDensityPressureDisplacement2D();
  }

  /////////////////        Collision Obstacles           /////////////////////////

  Primitives::Plane w;
  float boundMod = 0.9f;
  //Bottom
  w.normal = ngl::Vec3(0,1,0);
  w.centerOfPlane = ngl::Vec3(0,m_bounds.m_miny*boundMod,0);
  m_wallList.push_back(w);

  //Top
  w.normal = ngl::Vec3(0,-1,0);
  w.centerOfPlane = ngl::Vec3(0,m_bounds.m_maxy*boundMod,0);
  m_wallList.push_back(w);

  //Left
  w.normal = ngl::Vec3(1,0,0);
  w.centerOfPlane = ngl::Vec3(m_bounds.m_minx*boundMod,0,0);
  m_wallList.push_back(w);

  //Right
  w.normal = ngl::Vec3(-1,0,0);
  w.centerOfPlane = ngl::Vec3(m_bounds.m_maxx*boundMod,0,0);
  m_wallList.push_back(w);

  //Front
  w.normal = ngl::Vec3(0,0,-1);
  w.centerOfPlane = ngl::Vec3(0,0,m_bounds.m_maxz*boundMod);
  m_wallList.push_back(w);

  //Back
  w.normal = ngl::Vec3(0,0,1);
  w.centerOfPlane = ngl::Vec3(0,0,m_bounds.m_minz*boundMod);
  m_wallList.push_back(w);

  //ObstacleScene obstacleScene;
  //obstacleScene.addCuboidsToList(m_cuboidList);

  for (int i=0; i<m_rectangleList.size(); i++)
  {
    addRectangleDrawData(m_staticDrawData, m_rectangleList[i]);
  }

  for (unsigned int i=0; i<m_cuboidList.size(); i++)
  {
    addCuboidDrawData(m_staticDrawData, m_cuboidList[i]);
  }

  initializeBoundsVAO();
}

void SPH::applyExternalForces()
{
  if (m_2d && m_currentClickForce != 0)
  {
    for (unsigned int i=0; i<m_numParticles; i++)
    {
      Particle *p = &m_particleEmitter->m_particles[i];
      if (p->m_active)
      {
        ngl::Vec3 mouseForce = (m_mousePos - p->m_pos);
        if (mouseForce.lengthSquared() < (m_clickRadius*m_clickRadius))
        {
          mouseForce *= 1 - (mouseForce.lengthSquared() / (m_clickRadius*m_clickRadius));
          p->m_velocity += mouseForce * m_currentClickForce * m_dt;
        }
      }
    }
  }
}

void SPH::applyViscosity()
{
  for (unsigned int i=0; i<m_numParticles; i++)
  {
    Particle *p = &m_particleEmitter->m_particles[i];
    if (p->m_active)
    {
      for (unsigned int j=0; j<p->m_neighbours.size(); j++)
      {
        Particle *otherP = p->m_neighbours[j];
        ngl::Vec3 dir = otherP->m_pos - p->m_pos;
        float dist = dir.length();
        if (dist <= 0)
        {
          continue;
        }

        ngl::Vec3 dirNorm = dir;
        dirNorm.normalize();

        float u = (p->m_velocity - otherP->m_velocity).dot(dirNorm);

        if (u > 0.0f)
        {
          float a = 1 - (dist/m_kernel);
          ngl::Vec3 impulse = m_dt * a * (m_linearViscosity*u + m_quadraticViscosity*u*u)*dirNorm;
          p->m_velocity -= impulse*0.5f;
          otherP->m_velocity += impulse*0.5f;
        }
      }
    }
  }
}

void SPH::advection()
{
  //Save prev position, advance and check wall collisions
  for (unsigned int i=0; i<m_numParticles; i++)
  {
    Particle *p = &m_particleEmitter->m_particles[i];
    if (p->m_active)
    {
      p->m_velocity += m_gravity * m_dt;

      p->m_previousPos = p->m_pos;
      p->m_pos += p->m_velocity * m_dt;

      if (m_2d)
      {
        p->m_pos.m_z = 0;
        p->m_velocity.m_z = 0;
      }
    }
  }
}


void SPH::calculateDensityPressureDisplacement()
{
  //Run through every cell and each particle in that cell.
  for (unsigned int i=0; i<m_numCells; i++)
  {
    int cellX = m_vectorField->m_nodeList[i].m_cellIndex.x;
    int cellY = m_vectorField->m_nodeList[i].m_cellIndex.y;
    int cellZ = m_vectorField->m_nodeList[i].m_cellIndex.z;

    for (unsigned int j=0;j<m_vectorField->m_nodeList[i].m_particleList.size(); j++)
    {
      Particle *p  = m_vectorField->m_nodeList[i].m_particleList[j];
      if (p->m_active)
      {
        p->m_density = 0;
        p->m_nearDensity = 0;
        p->m_neighbours.clear();

        //Go through all surrounding and current cells of the current particle, calculating density and pressure based on other particles in those cells
        for (int x=-1; x<2; x++)
        {
          if (x + cellX < 0 || x + cellX > (int)m_gridSize-1)
          {
            continue;
          }

          for (int y=-1; y<2; y++)
          {
            if (y + cellY < 0 || y + cellY > (int)m_gridSize-1)
            {
              continue;
            }

            for (int z=-1; z<2; z++)
            {
              if (z + cellZ < 0 || z + cellZ > (int)m_gridSize-1)
              {
                continue;
              }

              unsigned int index = i + x + (y*m_gridSize) + (z*m_gridSize*m_gridZSize);

              for (unsigned int m=0; m<m_vectorField->m_nodeList[index].m_particleList.size(); m++)
              {
                Particle* otherP = m_vectorField->m_nodeList[index].m_particleList[m];
                if (otherP->m_active)
                {
                  ngl::Vec3 dir = otherP->m_pos - p->m_pos;
                  float distSquared = dir.lengthSquared();

                  if ((distSquared > m_kernel*m_kernel || distSquared < 0.0000001f))
                  {
                    continue;
                  }

                  float dist = dir.length();
                  float a = 1.0f - (dist/m_kernel);
                  p->m_density += a*a;
                  p->m_nearDensity += a*a*a;

                  p->m_neighbours.push_back(otherP);
                }
              }
            }
          }
        }

        pressureDisplacement(p);
      }
    }
  }
}

void SPH::calculateDensityPressureDisplacement2D()
{
  //Run through every cell and each particle in that cell.
  for (unsigned int i=0; i<m_numCells; i++)
  {

    for (unsigned int j=0;j<m_vectorField->m_nodeList[i].m_particleList.size(); j++)
    {
      Particle *p  = m_vectorField->m_nodeList[i].m_particleList[j];
      if (p->m_active)
      {
        p->m_density = 0;
        p->m_nearDensity = 0;
        p->m_neighbours.clear();

        //Go through all surrounding and current cells of the current particle, calculating density and pressure based on other particles in those cells
        for (int x=-1; x<2; x++)
        {
          for (int y=-1; y<2; y++)
          {
            unsigned int index = i + x + (y*m_gridSize);
            if (index >= m_numCells)
            {
              continue;
            }

            for (unsigned int m=0; m<m_vectorField->m_nodeList[index].m_particleList.size(); m++)
            {
              Particle* otherP = m_vectorField->m_nodeList[index].m_particleList[m];

              if (otherP->m_active)
              {
                ngl::Vec3 dir = otherP->m_pos - p->m_pos;
                float distSquared = dir.lengthSquared();

                if ((distSquared > m_kernel*m_kernel || distSquared < 0.0000001f))
                {
                  continue;
                }

                float dist = dir.length();
                float a = 1.0f - (dist/m_kernel);
                p->m_density += a*a;
                p->m_nearDensity += a*a*a;

                p->m_neighbours.push_back(otherP);
              }
            }
          }
        }
        pressureDisplacement(p);
      }
    }
  }
}

void SPH::pressureDisplacement(Particle *p)
{
  //Pressure
  p->m_pressure = (p->m_density-m_restDensity) * m_stiffness;
  p->m_nearPressure = m_nearStiffness * p->m_nearDensity;

  /// Citation
  /// The following code is from https://github.com/jingquanalex/fluid/blob/master/FluidScene/SPHClavet.cs
  float presTot = p->m_pressure + p->m_nearPressure;

  if (presTot < 0.000001f || presTot > 10000)
  {
    p->m_pressure = 0;
    p->m_nearPressure = 0;
  }
  /// End of citation

  //Displacement of particles
  ngl::Vec3 dispP = ngl::Vec3(0,0,0);
  for (unsigned int k=0; k<p->m_neighbours.size(); k++)
  {
    Particle* otherP = p->m_neighbours[k];
    ngl::Vec3 dir = otherP->m_pos - p->m_pos;
    float dist = dir.length();
    float a = 1 - (dist/m_kernel);
    dir.normalize();
    ngl::Vec3 d = m_dt*m_dt * (p->m_pressure*a + p->m_nearPressure*a*a) * dir;
    otherP->m_pos += d*0.5f;
    dispP -= d*0.5f;
  }
  p->m_pos += dispP; //Don't want a new position of p to be used for all the other neighbours so it is displaced after the loop from it's original position
}

void SPH::collisions()
{
  for (unsigned int i=0; i<m_numParticles; i++)
  {
    Particle *p = &m_particleEmitter->m_particles[i];

    if (p->m_active)
    {
      for (unsigned int j=0; j<m_sphereList.size(); j++)
      {
        particleSphereCollision(&m_sphereList[j],p);
      }

      for (unsigned int j=0; j<m_wallList.size(); j++)
      {
        infinitePlaneCollision(&m_wallList[j],p);
      }

      for (unsigned int j=0; j<m_rectangleList.size(); j++)
      {
        finitePlaneCollision(&m_rectangleList[j],p);
      }

      for (unsigned int j=0; j<m_cuboidList.size(); j++)
      {
        cuboidCollision(&m_cuboidList[j],p);
      }
    }
  }
}


void SPH::update()
{
  applyExternalForces();
  applyViscosity();
  advection();
  addParticlesToCells();
  if (!m_2d)
  {
    calculateDensityPressureDisplacement();
  }
  else
  {
    calculateDensityPressureDisplacement2D();
  }
  for (unsigned int i =0; i<m_numParticles; i++)
  {
    Particle *p = &m_particleEmitter->m_particles[i];
    p->m_velocity = (p->m_pos - p->m_previousPos) / m_dt;

    p->m_velocity.clamp(10);
  }

  //if (m_cuboidList.size() > 0)
    //rotateCuboid(m_cuboidList[0],0.4f);

  collisions();
}

//Particle to particle collision, assuming the particle bounding boxes are spheres
//Currently not in use
void SPH::particleSphereCollision(Primitives::Sphere *s, Particle *p)
{
  /// @brief Citation: The following section is based on:-
  /// https://studiofreya.com/3d-math-and-physics/simple-sphere-sphere-collision-detection-and-collision-response/

  ngl::Vec3 diff = s->m_position - p->m_pos;

  float distSquared = diff.lengthSquared();

  if (distSquared < (m_particleRadius*m_particleRadius)+(s->m_radius*s->m_radius))
  {
    //Collision
    //P1
    ngl::Vec3 norm = diff;
    norm.normalize();

    ngl::Vec3 v1 = p->m_velocity;
    float dotX1 = norm.dot(v1);

    ngl::Vec3 vel1 = norm * dotX1;
    ngl::Vec3 perp1 = v1 - vel1;

    p->m_velocity = (-vel1 + perp1)*m_bounce;
    /// end of citation

    float dist = diff.length();
    float moveDist =(s->m_radius + m_particleRadius) - dist;
    p->m_pos += -norm*moveDist;
  }
}

void SPH::infinitePlaneCollision(Primitives::Plane *_w, Particle *_p)
{
  //Project
  float project = fabs((_p->m_pos - _w->centerOfPlane).dot(_w->normal));

  if (project < m_particleRadius)
  {
    _p->m_velocity = _p->m_velocity - 2 * _p->m_velocity.dot(_w->normal) * _w->normal * m_bounce;
    _p->m_pos += (m_particleRadius-project) * _w->normal;
  }
}

void SPH::finitePlaneCollision(Primitives::Rectangle *_w, Particle *_p)
{
  float projectX = (_p->m_pos - _w->m_p1).dot(_w->m_p2 - _w-> m_p1) / (_w->m_width * _w->m_width); //Projection onto plane along p1 corner to p2 corner
  float projectZ = (_p->m_pos - _w->m_p1).dot(_w->m_p3 - _w-> m_p1) / (_w->m_height * _w->m_height);

  if (projectX >= 0.0f && projectX < 1.0f && projectZ >= 0.0f && projectZ < 1.0f)
  {
    float dist = fabs((_p->m_pos - _w->m_p1).dot(_w->m_normal));

    if (dist < m_particleRadius)
    {
      _p->m_velocity = _p->m_velocity - 2 * _p->m_velocity.dot(_w->m_normal) * _w->m_normal * m_bounce;
      _p->m_pos += (m_particleRadius-dist) * _w->m_normal;
    }
  }
}

void SPH::cuboidCollision(Primitives::Cuboid *_c, Particle *_p)
{
  for (int i=0; i<6; i++)
  {
    finitePlaneCollision(&_c->m_sides[i], _p);
  }
}

void SPH::rotateCuboid(Primitives::Cuboid &_c, float _deg)
{
  ngl::Mat3 rotMatrix;
  rotMatrix.identity();
  rotMatrix.rotateZ(_deg);
  for (int i=0; i<_c.m_sides.size(); i++)
  {
    for (int j=0; j<4; j++)
    {
      _c.m_sides[i].m_normal = _c.m_sides[i].m_normal * rotMatrix;
      _c.m_sides[i].m_p1 = _c.m_sides[i].m_p1 * rotMatrix;
      _c.m_sides[i].m_p2 = _c.m_sides[i].m_p2 * rotMatrix;
      _c.m_sides[i].m_p3 = _c.m_sides[i].m_p3 * rotMatrix;
      _c.m_sides[i].m_p4 = _c.m_sides[i].m_p4 * rotMatrix;
    }
  }
}

void SPH::draw(ngl::Mat4 _VP, ngl::Mat4 _V, GLuint _particleTextureID)
{
  glBindTexture(GL_TEXTURE_2D,_particleTextureID);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  m_particleEmitter->draw(_VP, _V);

  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  (*shader)["Color"]->use();

  shader->setShaderParamFromMat4("VP",_VP);

  m_vao->bind();
  m_vao->draw();
  m_vao->unbind();

  /*
  std::vector<ngl::Vec3> posData;
  addCuboidDrawData(posData,m_cuboidList[0]);

  ngl::VertexArrayObject *vao=ngl::VertexArrayObject::createVOA(GL_LINES);
  vao->bind();
  vao->setData(posData.size()*sizeof(ngl::Vec3),posData[0].m_x,GL_DYNAMIC_DRAW);
  vao->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(ngl::Vec3),0);
  vao->setNumIndices(posData.size());
  vao->draw();
  vao->unbind();
  vao->removeVOA();
  delete vao;*/
}

void SPH::addParticlesToCells()
{
  //Add particles to cells
  m_octree->clearParticlesFromTreeNodeAndVectorNodes(m_octree->m_rootNode);

  for (unsigned int i =0; i<m_numParticles; i++)
  {
    m_particleEmitter->m_particles[i].m_neighbours.clear();

    if (m_particleEmitter->m_particles[i].m_pos.lengthSquared() > m_distEdgeToCentreSquared)
    {
      m_particleEmitter->m_particles[i].m_pos = m_particleEmitter->m_particles[i].m_initialPos;
      m_particleEmitter->m_particles[i].m_velocity = m_particleEmitter->m_particles[i].m_initialVelocity;
    }

    if (m_particleEmitter->m_particles[i].m_active)
    {
      m_octree->addParticleToTreeAndVectorNodes(&m_particleEmitter->m_particles[i]);
    }
  }
}


void SPH::initializeBoundsVAO()
{
  m_vao=ngl::VertexArrayObject::createVOA(GL_LINES);
  m_vao->bind();
  m_vao->setData(m_staticDrawData.size()*sizeof(ngl::Vec3),m_staticDrawData[0].m_x);
  m_vao->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(ngl::Vec3),0);
  m_vao->setNumIndices(m_staticDrawData.size());

  m_vao->unbind();
}

void SPH::addBoundaryDrawData(std::vector<ngl::Vec3> &dataList, BoundingBox _bounds)
{
  if (m_2d)
    {
      ngl::Vec3 v1 = ngl::Vec3(_bounds.m_minx,_bounds.m_miny,0);
      ngl::Vec3 v2 = ngl::Vec3(_bounds.m_minx,_bounds.m_maxy,0);
      ngl::Vec3 v3 = ngl::Vec3(_bounds.m_maxx,_bounds.m_maxy,0);
      ngl::Vec3 v4 = ngl::Vec3(_bounds.m_maxx,_bounds.m_miny,0);

      dataList.push_back(v1);dataList.push_back(v2);
      dataList.push_back(v2);dataList.push_back(v3);
      dataList.push_back(v3);dataList.push_back(v4);
      dataList.push_back(v4);dataList.push_back(v1);
    }
    else
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
      dataList.push_back(v1);dataList.push_back(v2); //1 -> 2
      dataList.push_back(v2);dataList.push_back(v4); //2 -> 4 ...
      dataList.push_back(v4);dataList.push_back(v3);
      dataList.push_back(v3);dataList.push_back(v1);

      dataList.push_back(v1);dataList.push_back(v5);
      dataList.push_back(v5);dataList.push_back(v7);
      dataList.push_back(v7);dataList.push_back(v3);

      dataList.push_back(v4);dataList.push_back(v8);
      dataList.push_back(v8);dataList.push_back(v6);
      dataList.push_back(v6);dataList.push_back(v2);

      dataList.push_back(v8);dataList.push_back(v7);

      dataList.push_back(v6);dataList.push_back(v5);
    }
}

void SPH::addRectangleDrawData(std::vector<ngl::Vec3> &dataList, Primitives::Rectangle &_r)
{
  dataList.push_back(_r.m_p1);dataList.push_back(_r.m_p2);
  dataList.push_back(_r.m_p2);dataList.push_back(_r.m_p4);
  dataList.push_back(_r.m_p4);dataList.push_back(_r.m_p3);
  dataList.push_back(_r.m_p3);dataList.push_back(_r.m_p1);
}

void SPH::addCuboidDrawData(std::vector<ngl::Vec3> &dataList, Primitives::Cuboid &_c)
{
  if (m_2d)
  {
     addRectangleDrawData(dataList, _c.m_sides[4]);
  }
  else
  {
    for (unsigned int i=0; i<_c.m_sides.size(); i++)
    {
      addRectangleDrawData(dataList, _c.m_sides[i]);
    }
  }
}

void SPH::reset()
{
  m_particleEmitter->setNewParticleInitialPosition();

  for (unsigned int i =0; i<m_numParticles; i++)
  {
    m_particleEmitter->m_particles[i].reset();
  }
}

void SPH::mouseClick(bool _leftMouse, float _x, float _y)
{
  m_mousePos = ngl::Vec3(_x, _y, 0);

  if (_leftMouse)
  {
    m_currentClickForce = m_mouseClickForce;
  }
  else
  {
    m_currentClickForce = -m_mouseClickForce;
  }
}

void SPH::mouseRelease(bool _leftMouse)
{
  if (_leftMouse)
  {
    m_currentClickForce = 0;
  }
  else
  {
    m_currentClickForce = 0;
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
