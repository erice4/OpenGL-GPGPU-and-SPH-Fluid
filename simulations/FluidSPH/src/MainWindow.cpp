#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);
  m_nglScene = new NGLScene(this);
  m_ui->s_mainWindowGridLayout->addWidget(m_nglScene,0,0,4,1);

  connect(m_ui->m_simButton,SIGNAL(clicked(bool)),this,SLOT(setSimulate(bool)));
  connect(m_ui->m_resetButton,SIGNAL(clicked(bool)),this,SLOT(setReset()));

  connect(m_ui->m_kernel,SIGNAL(valueChanged(double)),this,SLOT(setKernel(double)));
  connect(m_ui->m_restDensity,SIGNAL(valueChanged(double)),this,SLOT(setRestDensity(double)));
  connect(m_ui->m_linViscosity,SIGNAL(valueChanged(double)),this,SLOT(setLinViscosity(double)));
  connect(m_ui->m_quadViscosity,SIGNAL(valueChanged(double)),this,SLOT(setQuadViscosity(double)));
  connect(m_ui->m_stiffness,SIGNAL(valueChanged(double)),this,SLOT(setStiffness(double)));
  connect(m_ui->m_nearStiffness,SIGNAL(valueChanged(double)),this,SLOT(setNearStiffness(double)));
  connect(m_ui->m_gravity,SIGNAL(valueChanged(double)),this,SLOT(setGravity(double)));
}

MainWindow::~MainWindow()
{
  delete m_ui;
  delete m_nglScene;
}

void MainWindow::setSimulate(bool _s)
{
  m_nglScene->toggleSimulate(_s);
}

void MainWindow::setReset()
{
  m_nglScene->toggleReset();
}

void MainWindow::setWindX(double _x)
{
  m_nglScene->toggleWindX((float)_x);
}

void MainWindow::setWindY(double _y)
{
  m_nglScene->toggleWindY((float)_y);
}

void MainWindow::setKernel(double _v)
{
  m_nglScene->toggleKernel(_v);
}

void MainWindow::setRestDensity(double _v)
{
  m_nglScene->toggleRestDensity(_v);
}

void MainWindow::setLinViscosity(double _v)
{
  m_nglScene->toggleLinViscosity(_v);
}

void MainWindow::setQuadViscosity(double _v)
{
  m_nglScene->toggleQuadViscosity(_v);
}

void MainWindow::setStiffness(double _v)
{
  m_nglScene->toggleStiffness(_v);
}

void MainWindow::setNearStiffness(double _v)
{
  m_nglScene->toggleNearStiffness(_v);
}

void MainWindow::setGravity(double _v)
{
  m_nglScene->toggleGravity(_v);
}
