/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *s_mainWindowGridLayout;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox;
    QPushButton *m_simButton;
    QPushButton *m_resetButton;
    QLabel *kernelLabel;
    QDoubleSpinBox *m_kernel;
    QLabel *restDensityLabel;
    QDoubleSpinBox *m_restDensity;
    QLabel *linViscLabel;
    QDoubleSpinBox *m_linViscosity;
    QLabel *uadViscLabel;
    QDoubleSpinBox *m_quadViscosity;
    QLabel *stiffnessLabel;
    QDoubleSpinBox *m_stiffness;
    QLabel *nearStiffnessLabel;
    QDoubleSpinBox *m_nearStiffness;
    QLabel *gravity_label;
    QDoubleSpinBox *m_gravity;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1366, 768);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(848, 480));
        MainWindow->setMaximumSize(QSize(16777215, 16777215));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        centralwidget->setMinimumSize(QSize(720, 405));
        s_mainWindowGridLayout = new QGridLayout(centralwidget);
        s_mainWindowGridLayout->setObjectName(QStringLiteral("s_mainWindowGridLayout"));
        horizontalSpacer = new QSpacerItem(1280, 720, QSizePolicy::Expanding, QSizePolicy::Minimum);

        s_mainWindowGridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMinimumSize(QSize(130, 0));
        groupBox->setAutoFillBackground(true);
        m_simButton = new QPushButton(groupBox);
        m_simButton->setObjectName(QStringLiteral("m_simButton"));
        m_simButton->setGeometry(QRect(0, 0, 127, 31));
        m_simButton->setCheckable(true);
        m_simButton->setChecked(true);
        m_resetButton = new QPushButton(groupBox);
        m_resetButton->setObjectName(QStringLiteral("m_resetButton"));
        m_resetButton->setGeometry(QRect(0, 40, 127, 31));
        kernelLabel = new QLabel(groupBox);
        kernelLabel->setObjectName(QStringLiteral("kernelLabel"));
        kernelLabel->setGeometry(QRect(0, 100, 127, 18));
        QFont font;
        font.setFamily(QStringLiteral("WenQuanYi Micro Hei Mono"));
        kernelLabel->setFont(font);
        m_kernel = new QDoubleSpinBox(groupBox);
        m_kernel->setObjectName(QStringLiteral("m_kernel"));
        m_kernel->setGeometry(QRect(0, 120, 127, 31));
        m_kernel->setMinimum(0.4);
        m_kernel->setSingleStep(0.1);
        m_kernel->setValue(0.8);
        restDensityLabel = new QLabel(groupBox);
        restDensityLabel->setObjectName(QStringLiteral("restDensityLabel"));
        restDensityLabel->setGeometry(QRect(0, 160, 127, 21));
        m_restDensity = new QDoubleSpinBox(groupBox);
        m_restDensity->setObjectName(QStringLiteral("m_restDensity"));
        m_restDensity->setGeometry(QRect(0, 180, 127, 31));
        m_restDensity->setValue(10);
        linViscLabel = new QLabel(groupBox);
        linViscLabel->setObjectName(QStringLiteral("linViscLabel"));
        linViscLabel->setGeometry(QRect(0, 220, 127, 21));
        m_linViscosity = new QDoubleSpinBox(groupBox);
        m_linViscosity->setObjectName(QStringLiteral("m_linViscosity"));
        m_linViscosity->setGeometry(QRect(0, 240, 127, 31));
        m_linViscosity->setValue(1.4);
        uadViscLabel = new QLabel(groupBox);
        uadViscLabel->setObjectName(QStringLiteral("uadViscLabel"));
        uadViscLabel->setGeometry(QRect(0, 280, 127, 21));
        m_quadViscosity = new QDoubleSpinBox(groupBox);
        m_quadViscosity->setObjectName(QStringLiteral("m_quadViscosity"));
        m_quadViscosity->setGeometry(QRect(0, 300, 127, 31));
        m_quadViscosity->setValue(2);
        stiffnessLabel = new QLabel(groupBox);
        stiffnessLabel->setObjectName(QStringLiteral("stiffnessLabel"));
        stiffnessLabel->setGeometry(QRect(0, 340, 94, 21));
        m_stiffness = new QDoubleSpinBox(groupBox);
        m_stiffness->setObjectName(QStringLiteral("m_stiffness"));
        m_stiffness->setGeometry(QRect(0, 360, 127, 31));
        m_stiffness->setValue(5.6);
        nearStiffnessLabel = new QLabel(groupBox);
        nearStiffnessLabel->setObjectName(QStringLiteral("nearStiffnessLabel"));
        nearStiffnessLabel->setGeometry(QRect(0, 400, 94, 21));
        m_nearStiffness = new QDoubleSpinBox(groupBox);
        m_nearStiffness->setObjectName(QStringLiteral("m_nearStiffness"));
        m_nearStiffness->setGeometry(QRect(0, 420, 127, 31));
        m_nearStiffness->setValue(5);
        gravity_label = new QLabel(groupBox);
        gravity_label->setObjectName(QStringLiteral("gravity_label"));
        gravity_label->setGeometry(QRect(0, 460, 66, 21));
        m_gravity = new QDoubleSpinBox(groupBox);
        m_gravity->setObjectName(QStringLiteral("m_gravity"));
        m_gravity->setGeometry(QRect(0, 480, 127, 31));
        m_gravity->setMinimum(-50);
        m_gravity->setMaximum(50);
        m_gravity->setValue(-10);

        s_mainWindowGridLayout->addWidget(groupBox, 1, 3, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1366, 27));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "SPH Fluid", 0));
        groupBox->setTitle(QString());
        m_simButton->setText(QApplication::translate("MainWindow", "Run", 0));
        m_resetButton->setText(QApplication::translate("MainWindow", "Reset", 0));
        kernelLabel->setText(QApplication::translate("MainWindow", "Kernel", 0));
        restDensityLabel->setText(QApplication::translate("MainWindow", "Rest Density", 0));
        linViscLabel->setText(QApplication::translate("MainWindow", "Linear Viscosity", 0));
        uadViscLabel->setText(QApplication::translate("MainWindow", "Quadratic Viscosity", 0));
        stiffnessLabel->setText(QApplication::translate("MainWindow", "Stiffness", 0));
        nearStiffnessLabel->setText(QApplication::translate("MainWindow", "Near Stiffness", 0));
        gravity_label->setText(QApplication::translate("MainWindow", "Gravity", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
