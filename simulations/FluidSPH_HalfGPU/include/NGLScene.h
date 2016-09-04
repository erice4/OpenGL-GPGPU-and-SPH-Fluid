#ifndef NGLSCENE_H__
#define NGLSCENE_H__
#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/Transformation.h>
#include <ngl/Text.h>
#include <QOpenGLWidget>
#include <SPH.h>
#include <memory>
#include <QTime>

//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @brief This class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
/// @author Ewan Rice
/// @version 1.0
/// @date 28/01/16
/// @class NGLScene
/// @brief Our main glwindow widget for NGL applications all drawing elements are
/// put in this file.
/// This class was originally written by Jon Macey and then modified.
//----------------------------------------------------------------------------------------------------------------------

class NGLScene : public QOpenGLWidget
{
Q_OBJECT
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor for our NGL drawing class
    /// @param [in] parent the parent window to the class
    //----------------------------------------------------------------------------------------------------------------------
    NGLScene(QWidget *_parent);
    //NGLScene();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor must close down ngl and release OpenGL resources
    //----------------------------------------------------------------------------------------------------------------------
    ~NGLScene();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief change the bool for whether to run the update or not
    void toggleSimulate(bool _s);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief reset the SPH simulation
    void toggleReset();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief change the X value of the wind
    void toggleWindX(float _x);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief change the Y value of the wind
    void toggleWindY(float _y);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief change the kernal length
    void toggleKernel(float _v);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief change the rest density
    void toggleRestDensity(float _v);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief change the linear viscosity
    void toggleLinViscosity(float _v);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief change the quadratic viscosity
    void toggleQuadViscosity(float _v);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief change the linear viscosity
    void toggleStiffness(float _v);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief change the linear viscosity
    void toggleNearStiffness(float _v);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief change the linear viscosity
    void toggleGravity(float _v);
    //----------------------------------------------------------------------------------------------------------------------

private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we resize the window
    //----------------------------------------------------------------------------------------------------------------------
    // Qt 5.5.1 must have this implemented and uses it
    //void resizeGL(QResizeEvent *_event);
    // Qt 5.x uses this instead! http://doc.qt.io/qt-5/qopenglwindow.html#resizeGL
    void resizeGL(int _w, int _h);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set up all shaders to be used
    //----------------------------------------------------------------------------------------------------------------------
    void createShaders();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief load the textures used in the program
    //----------------------------------------------------------------------------------------------------------------------
    void loadTexture();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Qt Event called when a key is pressed
    /// @param [in] _event the Qt event to query for size etc
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called every time a mouse is moved
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseMoveEvent (QMouseEvent * _event );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is pressed
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mousePressEvent ( QMouseEvent *_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is released
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent ( QMouseEvent *_event );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse wheel is moved
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void wheelEvent( QWheelEvent *_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief timer called every at a give frame rate
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void timerEvent(QTimerEvent *);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the x rotation mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_spinXFace;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the y rotation mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_spinYFace;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if the mouse button is pressed when dragging
    //----------------------------------------------------------------------------------------------------------------------
    bool m_rotatingCamera;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if the Right mouse button is pressed when dragging
    //----------------------------------------------------------------------------------------------------------------------
    bool m_translatingCamera;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous x mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_origX;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous y mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_origY;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous x mouse value for Position changes
    //----------------------------------------------------------------------------------------------------------------------
    int m_origXPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous y mouse value for Position changes
    //----------------------------------------------------------------------------------------------------------------------
    int m_origYPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the global mouse transforms
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Mat4 m_mouseGlobalTX;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the model position for mouse movement
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_modelPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief window width
    //----------------------------------------------------------------------------------------------------------------------
    int m_width;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief window height
    //----------------------------------------------------------------------------------------------------------------------
    int m_height;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the particle update timer
    int m_particleTimer;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief time in seconds between frames
    float m_timeInterval;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief particle texture id
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_textureID;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief number of particles created
    int m_numParticles;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ngl camera object
    ngl::Camera m_camera;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief text to display fps
    std::unique_ptr<ngl::Text> m_text;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the fps to draw
    //----------------------------------------------------------------------------------------------------------------------
    int m_fps;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief number of frames for the fps counter
    //----------------------------------------------------------------------------------------------------------------------
    int m_frames;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief timer for re-draw
    //----------------------------------------------------------------------------------------------------------------------
    QTime m_timer;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to pause/unpause simulation
    //----------------------------------------------------------------------------------------------------------------------
    bool m_active;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to display particles
    //----------------------------------------------------------------------------------------------------------------------
    bool m_displayParticles;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to display vector field
    //----------------------------------------------------------------------------------------------------------------------
    bool m_displayVectorField;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to display octree
    //----------------------------------------------------------------------------------------------------------------------
    bool m_displayOctree;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief SPH object
    //----------------------------------------------------------------------------------------------------------------------
    std::unique_ptr<SPH> m_sph;

protected:
    /// @brief the initialize class is called once when the window is created and we have a valid GL context
    /// use this to setup any default GL stuff
    //----------------------------------------------------------------------------------------------------------------------
    void initializeGL();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we want to draw the scene
    //----------------------------------------------------------------------------------------------------------------------
    void paintGL();
};



#endif
