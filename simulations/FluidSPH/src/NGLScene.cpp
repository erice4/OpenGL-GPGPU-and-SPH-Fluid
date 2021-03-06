#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <ngl/Texture.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Material.h>
#include <QGLWidget>

NGLScene::NGLScene(QWidget *_parent) : QOpenGLWidget(_parent)
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  //setTitle("Rain and Snow");
  //srand(time(0));

  setFocus();
  this->resize(_parent->size());

  m_rotatingCamera=false;
  m_spinXFace=0;
  m_spinYFace=0;

  m_fps=0;
  m_frames=0;

  m_active = true;

  m_timer.start();
}

NGLScene::~NGLScene()
{

}

/*void NGLScene::resizeGL(QResizeEvent *_event)
{
  m_width=_event->size().width()*devicePixelRatio();
  m_height=_event->size().height()*devicePixelRatio();
  m_camera.setShape(45.0f,(float)width()/height(),0.05f,350.0f);
}*/

void NGLScene::resizeGL(int _w , int _h)
{
  m_width=_w*devicePixelRatio();
  m_height=_h*devicePixelRatio();
  m_camera.setShape(45.0f,(float)_w/_h,0.05f,350.0f);
}


void NGLScene::initializeGL()
{
  // we need to initialise the NGL lib which will load all of the OpenGL functions, this must
  // be done once we have a valid GL context but before we call any GL commands. If we dont do
  // this everything will crash
  ngl::NGLInit::instance();
  glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);

  //Camera setup
  ngl::Vec3 from(0,0,25);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  m_camera.set(from,to,up);
  m_camera.setShape(45,16.0f/9.0f,0.1,3000);
  createShaders();

  ngl::Material m(ngl::STDMAT::BLACKPLASTIC);
  m.setDiffuse(ngl::Colour(0.2f,0.3f,0.4f,1));
  m.setSpecularExponent(50.0f);
  m.loadToShader("material");

  m_transform.reset();
  m_transform.setRotation(90,0,0);
  m_transform.setPosition(0,0,-25);

  m_light.reset(new ngl::Light(ngl::Vec3(-10,40,90),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::LightModes::POINTLIGHT));
  ngl::Mat4 iv=m_camera.getViewMatrix();
  iv.transpose();
  m_light->setTransform(iv);
  m_light->loadToShader("light");

  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->createTrianglePlane("plane",140,140,80,80,ngl::Vec3(0,1,0));

  m_text.reset(new ngl::Text(QFont("Arial",14)));
  m_text->setScreenSize(width(),height());
  m_text->setColour(1,0,0);

  loadTexture();

  m_timeInterval = 0.025f;
  m_particleTimer = startTimer((int)(m_timeInterval*100));

  m_sph.reset(new SPH(m_timeInterval));

  glPointSize(4);
}

void NGLScene::createShaders()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->createShaderProgram("Particle");

  shader->attachShader("ParticleVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("ParticleFragment",ngl::ShaderType::FRAGMENT);
  shader->attachShader("ParticleGeometry",ngl::ShaderType::GEOMETRY);

  shader->loadShaderSource("ParticleVertex","shaders/particleVert.glsl");
  shader->loadShaderSource("ParticleFragment","shaders/particleFrag.glsl");
  shader->loadShaderSource("ParticleGeometry","shaders/particleGeo.glsl");

  shader->compileShader("ParticleVertex");
  shader->compileShader("ParticleFragment");
  shader->compileShader("ParticleGeometry");
  shader->attachShaderToProgram("Particle","ParticleVertex");
  shader->attachShaderToProgram("Particle","ParticleFragment");
  shader->attachShaderToProgram("Particle","ParticleGeometry");

  shader->linkProgramObject("Particle");
  (*shader)["Particle"]->use();


  shader->createShaderProgram("Color");

  shader->attachShader("ColorVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("ColorFragment",ngl::ShaderType::FRAGMENT);

  shader->loadShaderSource("ColorVertex","shaders/colorVert.glsl");
  shader->loadShaderSource("ColorFragment","shaders/colorFrag.glsl");

  shader->attachShaderToProgram("Color","ColorVertex");
  shader->attachShaderToProgram("Color","ColorFragment");

  shader->compileShader("ColorVertex");
  shader->compileShader("ColorFragment");

  shader->linkProgramObject("Color");
  (*shader)["Color"]->use();


  shader->createShaderProgram("Phong");

  shader->attachShader("PhongVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("PhongFragment",ngl::ShaderType::FRAGMENT);

  shader->loadShaderSource("PhongVertex","shaders/phongVert.glsl");
  shader->loadShaderSource("PhongFragment","shaders/phongFrag.glsl");

  shader->compileShader("PhongVertex");
  shader->compileShader("PhongFragment");

  shader->attachShaderToProgram("Phong", "PhongVertex");
  shader->attachShaderToProgram("Phong", "PhongFragment");

  shader->linkProgramObject("Phong");
  (*shader)["Phong"]->use();
}

void NGLScene::loadTexture()
{
  ///@brief originally written by Jon Macey
  QImage image;

  bool loaded=image.load("textures/laser.png");

  if(loaded == true)
  {
    int width=image.width();
    int height=image.height();
    // note this method is depracted as it uses the Older GLWidget but does work
    image = QGLWidget::convertToGLFormat(image);

    glGenTextures(1,&m_textureID);
    glBindTexture(GL_TEXTURE_2D,m_textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  glDisable(GL_CULL_FACE);

  /// end of citation
}

void NGLScene::timerEvent(QTimerEvent *_event )
{
  if (m_active)
  {
    if(_event->timerId() == m_particleTimer)
    {
      if (m_sph->m_2d)
      {
        if (m_leftMouseClicked)
        {
          //std::cout<< m_origX <<"\n";
          ngl::Vec3 w = getWorldSpace(m_origX,m_origY) / 150;
          m_sph->mouseClick(true,w.m_x, w.m_y);
        }
        else if (m_rightMouseClicked)
        {
          ngl::Vec3 w = getWorldSpace(m_origX,m_origY) / 150;
          m_sph->mouseClick(false,w.m_x, w.m_y);
        }
      }

      m_sph->update();

      if (m_timer.elapsed() > 1000.0)
      {
        m_fps = m_frames;
        m_frames=0;
        m_timer.restart();
      }
    }
    update();
  }
}

void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_width,m_height);

  // Rotation based on the mouse position for our global
  // transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_spinXFace);
  rotY.rotateY(m_spinYFace);
  // multiply the rotations
  m_mouseGlobalTX=rotY*rotX;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

  ngl::Mat4 vp;
  vp = m_mouseGlobalTX*m_camera.getVPMatrix();
  ngl::Mat4 v = m_mouseGlobalTX * m_camera.getViewMatrix();

  if (m_sph->m_2d)
  {
    ngl::Mat3 normalMatrix = m_transform.getMatrix()*v;
    normalMatrix.inverse();

    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["Phong"]->use();
    m_light->loadToShader("light");

    shader->setShaderParam3f("viewerPos",m_camera.getEye().m_x,m_camera.getEye().m_y,m_camera.getEye().m_z);
    shader->setShaderParamFromMat4("MV",m_transform.getMatrix()*v);
    shader->setShaderParamFromMat4("MVP",m_transform.getMatrix()*vp);
    shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
    shader->setShaderParamFromMat4("M",m_transform.getMatrix()*m_mouseGlobalTX);

    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
    prim->draw("plane");
  }


  m_sph->draw(vp, v, m_textureID);


  QString text=QString("%2 fps").arg(m_fps);
  m_text->renderText(10,20,text);

  m_frames++;
}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent (QMouseEvent * _event)
{
  if (m_sph->m_2d)
  {
    m_origX = _event->x();
    m_origY = _event->y();
    update();
  }
  else
  {
    if (m_rotatingCamera && _event->buttons() == Qt::LeftButton)
    {
      int diffx=_event->x()-m_origX;
      int diffy=_event->y()-m_origY;
      m_spinXFace -= (float) 0.5f * diffy;
      m_spinYFace += (float) 0.5f * diffx;
      m_origX = _event->x();
      m_origY = _event->y();
      update();
    }
    else if (m_translatingCamera && _event->buttons() == Qt::RightButton)
    {
      int diffX = (int)(_event->x() - m_origXPos);
      int diffY = (int)(_event->y() - m_origYPos);
      m_origXPos=_event->x();
      m_origYPos=_event->y();
      m_modelPos.m_x += 0.2 * diffX;
      m_modelPos.m_y -= 0.2 * diffY;
      update();
    }
  }
}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent ( QMouseEvent * _event)
{
  if(_event->button() == Qt::LeftButton)
  {
    m_origX = _event->x();
    m_origY = _event->y();
    m_rotatingCamera =true;
    m_leftMouseClicked = true;
  }
  // right mouse translate mode
  else if(_event->button() == Qt::RightButton)
  {
    m_origXPos = _event->x();
    m_origYPos = _event->y();
    m_translatingCamera=true;
    m_rightMouseClicked = true;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseReleaseEvent ( QMouseEvent * _event )
{
  if (_event->button() == Qt::LeftButton)
  {
    m_rotatingCamera=false;
    m_sph->mouseRelease(true);
    m_leftMouseClicked = false;
  }
        // right mouse translate mode
  if (_event->button() == Qt::RightButton)
  {
    m_translatingCamera=false;
    m_sph->mouseRelease(false);
    m_rightMouseClicked = false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::wheelEvent(QWheelEvent *_event)
{
  if(_event->delta() > 0)
  {
    m_modelPos.m_z+=2.1f;
  }
  else if(_event->delta() <0 )
  {
    m_modelPos.m_z-=2.1f;
  }
  update();
}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
    // escape key to quite
    case Qt::Key_Escape :
      QGuiApplication::exit(EXIT_SUCCESS);
      break;
    // turn on wirframe rendering
    case Qt::Key_S :
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      break;
    case Qt::Key_Up :
      break;
    case Qt::Key_Down :
      break;
    default :
    break;
  }
  // finally update the GLWindow and re-draw

    update();
}

ngl::Vec3 NGLScene::getWorldSpace(int _x, int _y)
{
  /*ngl::Mat4 t=m_camera.getProjectionMatrix();
  ngl::Mat4 v=m_camera.getViewMatrix();
  // as ngl:: and OpenGL use different formats need to transpose the matrix.
  t.transpose();
  v.transpose();
  ngl::Mat4 inverse=( t*v).inverse();

  ngl::Vec4 tmp(0,0,1.0f,1.0f);
  // convert into NDC
  tmp.m_x=(2.0f * _x) / width() - 1.0f;
  tmp.m_y=1.0f - (2.0f * _y) / height();
  // scale by inverse MV * Project transform
  ngl::Vec4 obj=inverse*tmp;
  // Scale by w
  obj/=obj.m_w;
  return obj.toVec3();*/

  return ngl::unProject(ngl::Vec3(_x,_y,1.0f),m_camera.getViewMatrix(), m_camera.getProjectionMatrix(), ngl::Vec4(0,0,width(),height()));
}

void NGLScene::toggleSimulate(bool _s)
{
  m_active = _s;
}

void NGLScene::toggleReset()
{
  m_sph->reset();
}

void NGLScene::toggleWindX(float _x)
{
  m_sph->setWindX(_x);
}

void NGLScene::toggleWindY(float _y)
{
  m_sph->setWindY(_y);
}

void NGLScene::toggleKernel(float _v)
{
  m_sph->setKernel(_v);
}

void NGLScene::toggleRestDensity(float _v)
{
  m_sph->setRestDensity(_v);
}

void NGLScene::toggleLinViscosity(float _v)
{
  m_sph->setLinViscosity(_v);
}

void NGLScene::toggleQuadViscosity(float _v)
{
  m_sph->setQuadViscosity(_v);
}

void NGLScene::toggleStiffness(float _v)
{
  m_sph->setStiffness(_v);
}

void NGLScene::toggleNearStiffness(float _v)
{
  m_sph->setNearStiffness(_v);
}

void NGLScene::toggleGravity(float _v)
{
  m_sph->setGravity(_v);
}
