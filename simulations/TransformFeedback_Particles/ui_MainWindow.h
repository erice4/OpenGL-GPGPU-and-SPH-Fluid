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
    QPushButton *m_simButton;
    QPushButton *m_resetButton;
    QGroupBox *groupBox;
    QLabel *wind_label;
    QLabel *xLabel;
    QDoubleSpinBox *m_windXInput;
    QLabel *yLabel;
    QDoubleSpinBox *m_windYInput;
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
        horizontalSpacer = new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        s_mainWindowGridLayout->addItem(horizontalSpacer, 0, 0, 18, 1);

        m_simButton = new QPushButton(centralwidget);
        m_simButton->setObjectName(QStringLiteral("m_simButton"));
        m_simButton->setCheckable(true);
        m_simButton->setChecked(true);

        s_mainWindowGridLayout->addWidget(m_simButton, 1, 3, 1, 1);

        m_resetButton = new QPushButton(centralwidget);
        m_resetButton->setObjectName(QStringLiteral("m_resetButton"));

        s_mainWindowGridLayout->addWidget(m_resetButton, 2, 3, 1, 1);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        groupBox->setMinimumSize(QSize(120, 800));
        wind_label = new QLabel(groupBox);
        wind_label->setObjectName(QStringLiteral("wind_label"));
        wind_label->setGeometry(QRect(50, 20, 35, 20));
        wind_label->setMaximumSize(QSize(16777215, 20));
        wind_label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        xLabel = new QLabel(groupBox);
        xLabel->setObjectName(QStringLiteral("xLabel"));
        xLabel->setGeometry(QRect(0, 40, 20, 20));
        xLabel->setMaximumSize(QSize(20, 20));
        m_windXInput = new QDoubleSpinBox(groupBox);
        m_windXInput->setObjectName(QStringLiteral("m_windXInput"));
        m_windXInput->setGeometry(QRect(0, 60, 93, 31));
        m_windXInput->setMaximumSize(QSize(100, 16777215));
        m_windXInput->setMinimum(-100);
        m_windXInput->setMaximum(100);
        yLabel = new QLabel(groupBox);
        yLabel->setObjectName(QStringLiteral("yLabel"));
        yLabel->setGeometry(QRect(0, 100, 20, 20));
        yLabel->setMaximumSize(QSize(20, 20));
        m_windYInput = new QDoubleSpinBox(groupBox);
        m_windYInput->setObjectName(QStringLiteral("m_windYInput"));
        m_windYInput->setGeometry(QRect(0, 130, 93, 31));
        m_windYInput->setMaximumSize(QSize(100, 16777215));
        m_windYInput->setDecimals(2);
        m_windYInput->setMinimum(-10);
        m_windYInput->setMaximum(10);

        s_mainWindowGridLayout->addWidget(groupBox, 3, 3, 1, 1);

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
        m_simButton->setText(QApplication::translate("MainWindow", "Simulate", 0));
        m_resetButton->setText(QApplication::translate("MainWindow", "Reset", 0));
        groupBox->setTitle(QString());
        wind_label->setText(QApplication::translate("MainWindow", "Wind", 0));
        xLabel->setText(QApplication::translate("MainWindow", "X", 0));
        yLabel->setText(QApplication::translate("MainWindow", "Y", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
