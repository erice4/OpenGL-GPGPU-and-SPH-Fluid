#ifndef MAINWINDOW_H__
#define MAINWINDOW_H__

#include <QMainWindow>
#include "NGLScene.h"
#include <iostream>

//----------------------------------------------------------------------------------------------------------------------
/// @file MainWindow.h
/// @brief This class inherits from the QMainWindow and connects the UI buttons with set methods in NGLScene.
/// @author Ewan Rice
/// @version 1.0
/// @date 28/01/16
/// @class MainWindow
/// @brief This class inherits from the QMainWindow and connects the UI buttons with set methods in NGLScene.
/// This class was originally written by Jon Macey, and then modified.
/// https://github.com/NCCA/MassSpring
//----------------------------------------------------------------------------------------------------------------------

namespace Ui
{
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  Ui::MainWindow *m_ui;
  NGLScene *m_nglScene;

private slots:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief slot to set simulate
  void setSimulate(bool _s);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief slot to set display flag for particles
  void setReset();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief slot to set wind
  void setWindX(double _x);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief slot to set wind
  void setWindY(double _y);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set the kernel length
  void setKernel(double _v);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief set the rest density
  void setRestDensity(double _v);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief set the linear viscosity
  void setLinViscosity(double _v);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief set the quadratic viscosity
  void setQuadViscosity(double _v);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief set the stiffness
  void setStiffness(double _v);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief set the near stiffness
  void setNearStiffness(double _v);
  //-----------------------------------------------------------------------------------------------------------------------------------
  /// @brief set the gravity
  void setGravity(double _v);
  //-----------------------------------------------------------------------------------------------------------------------------------
};

#endif // MAINWINDOW_H__

