/********************************************************************************
** Form generated from reading UI file 'EconomicEngineDebugGUI.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ECONOMICENGINEDEBUGGUI_H
#define UI_ECONOMICENGINEDEBUGGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_EconomicEngineDebugGUIClass
{
public:
    QWidget *centralWidget;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *layGraph;
    QVBoxLayout *layGraphParam;
    QGridLayout *layChBx;
    QFrame *line;
    QGridLayout *LaySliders;
    QLabel *labSpeed;
    QSlider *horSlidSpeed;
    QSlider *horSlidZoomXAxis;
    QLabel *labZomXAxis;
    QLabel *labStep;
    QSlider *horSlidStep;
    QFrame *line_2;
    QHBoxLayout *layButton;
    QVBoxLayout *layRadButton;
    QRadioButton *radBRealTime;
    QRadioButton *radStepByStep;
    QVBoxLayout *LayPushButton;
    QPushButton *pBStart;
    QPushButton *pBReset;
    QCustomPlot *customPlot;
    QSlider *horizontalXSlider;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *EconomicEngineDebugGUIClass)
    {
        if (EconomicEngineDebugGUIClass->objectName().isEmpty())
            EconomicEngineDebugGUIClass->setObjectName(QString::fromUtf8("EconomicEngineDebugGUIClass"));
        EconomicEngineDebugGUIClass->resize(1200, 680);
        EconomicEngineDebugGUIClass->setMinimumSize(QSize(1180, 680));
        EconomicEngineDebugGUIClass->setMaximumSize(QSize(1200, 680));
        EconomicEngineDebugGUIClass->setAutoFillBackground(false);
        EconomicEngineDebugGUIClass->setAnimated(true);
        centralWidget = new QWidget(EconomicEngineDebugGUIClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayoutWidget_3 = new QWidget(centralWidget);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(10, 10, 1179, 341));
        layGraph = new QHBoxLayout(horizontalLayoutWidget_3);
        layGraph->setSpacing(6);
        layGraph->setContentsMargins(11, 11, 11, 11);
        layGraph->setObjectName(QString::fromUtf8("layGraph"));
        layGraph->setContentsMargins(0, 0, 0, 0);
        layGraphParam = new QVBoxLayout();
        layGraphParam->setSpacing(6);
        layGraphParam->setObjectName(QString::fromUtf8("layGraphParam"));
        layChBx = new QGridLayout();
        layChBx->setSpacing(6);
        layChBx->setObjectName(QString::fromUtf8("layChBx"));

        layGraphParam->addLayout(layChBx);

        line = new QFrame(horizontalLayoutWidget_3);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        layGraphParam->addWidget(line);

        LaySliders = new QGridLayout();
        LaySliders->setSpacing(6);
        LaySliders->setObjectName(QString::fromUtf8("LaySliders"));
        labSpeed = new QLabel(horizontalLayoutWidget_3);
        labSpeed->setObjectName(QString::fromUtf8("labSpeed"));

        LaySliders->addWidget(labSpeed, 1, 0, 1, 1);

        horSlidSpeed = new QSlider(horizontalLayoutWidget_3);
        horSlidSpeed->setObjectName(QString::fromUtf8("horSlidSpeed"));
        horSlidSpeed->setCursor(QCursor(Qt::PointingHandCursor));
        horSlidSpeed->setMinimum(1);
        horSlidSpeed->setMaximum(50);
        horSlidSpeed->setValue(5);
        horSlidSpeed->setOrientation(Qt::Horizontal);
        horSlidSpeed->setInvertedAppearance(false);
        horSlidSpeed->setInvertedControls(false);

        LaySliders->addWidget(horSlidSpeed, 1, 1, 1, 1);

        horSlidZoomXAxis = new QSlider(horizontalLayoutWidget_3);
        horSlidZoomXAxis->setObjectName(QString::fromUtf8("horSlidZoomXAxis"));
        horSlidZoomXAxis->setCursor(QCursor(Qt::PointingHandCursor));
        horSlidZoomXAxis->setMinimum(4);
        horSlidZoomXAxis->setMaximum(100);
        horSlidZoomXAxis->setSingleStep(1);
        horSlidZoomXAxis->setValue(20);
        horSlidZoomXAxis->setOrientation(Qt::Horizontal);
        horSlidZoomXAxis->setInvertedAppearance(true);
        horSlidZoomXAxis->setInvertedControls(true);

        LaySliders->addWidget(horSlidZoomXAxis, 0, 1, 1, 1);

        labZomXAxis = new QLabel(horizontalLayoutWidget_3);
        labZomXAxis->setObjectName(QString::fromUtf8("labZomXAxis"));

        LaySliders->addWidget(labZomXAxis, 0, 0, 1, 1);

        labStep = new QLabel(horizontalLayoutWidget_3);
        labStep->setObjectName(QString::fromUtf8("labStep"));

        LaySliders->addWidget(labStep, 2, 0, 1, 1);

        horSlidStep = new QSlider(horizontalLayoutWidget_3);
        horSlidStep->setObjectName(QString::fromUtf8("horSlidStep"));
        horSlidStep->setCursor(QCursor(Qt::PointingHandCursor));
        horSlidStep->setMinimum(1);
        horSlidStep->setMaximum(1000);
        horSlidStep->setValue(1);
        horSlidStep->setOrientation(Qt::Horizontal);
        horSlidStep->setInvertedAppearance(false);
        horSlidStep->setInvertedControls(false);

        LaySliders->addWidget(horSlidStep, 2, 1, 1, 1);


        layGraphParam->addLayout(LaySliders);

        line_2 = new QFrame(horizontalLayoutWidget_3);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        layGraphParam->addWidget(line_2);

        layButton = new QHBoxLayout();
        layButton->setSpacing(6);
        layButton->setObjectName(QString::fromUtf8("layButton"));
        layRadButton = new QVBoxLayout();
        layRadButton->setSpacing(4);
        layRadButton->setObjectName(QString::fromUtf8("layRadButton"));
        layRadButton->setSizeConstraint(QLayout::SetNoConstraint);
        radBRealTime = new QRadioButton(horizontalLayoutWidget_3);
        radBRealTime->setObjectName(QString::fromUtf8("radBRealTime"));
        radBRealTime->setLayoutDirection(Qt::LeftToRight);
        radBRealTime->setAutoFillBackground(false);
        radBRealTime->setChecked(true);

        layRadButton->addWidget(radBRealTime);

        radStepByStep = new QRadioButton(horizontalLayoutWidget_3);
        radStepByStep->setObjectName(QString::fromUtf8("radStepByStep"));

        layRadButton->addWidget(radStepByStep);


        layButton->addLayout(layRadButton);

        LayPushButton = new QVBoxLayout();
        LayPushButton->setSpacing(6);
        LayPushButton->setObjectName(QString::fromUtf8("LayPushButton"));
        pBStart = new QPushButton(horizontalLayoutWidget_3);
        pBStart->setObjectName(QString::fromUtf8("pBStart"));
        pBStart->setCheckable(true);

        LayPushButton->addWidget(pBStart);

        pBReset = new QPushButton(horizontalLayoutWidget_3);
        pBReset->setObjectName(QString::fromUtf8("pBReset"));
        pBReset->setCheckable(true);

        LayPushButton->addWidget(pBReset);


        layButton->addLayout(LayPushButton);


        layGraphParam->addLayout(layButton);


        layGraph->addLayout(layGraphParam);

        customPlot = new QCustomPlot(horizontalLayoutWidget_3);
        customPlot->setObjectName(QString::fromUtf8("customPlot"));
        customPlot->setMinimumSize(QSize(830, 300));
        customPlot->setMaximumSize(QSize(830, 300));

        layGraph->addWidget(customPlot);

        horizontalXSlider = new QSlider(centralWidget);
        horizontalXSlider->setObjectName(QString::fromUtf8("horizontalXSlider"));
        horizontalXSlider->setGeometry(QRect(350, 360, 841, 22));
        horizontalXSlider->setOrientation(Qt::Horizontal);
        EconomicEngineDebugGUIClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(EconomicEngineDebugGUIClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        statusBar->setEnabled(true);
        EconomicEngineDebugGUIClass->setStatusBar(statusBar);

        retranslateUi(EconomicEngineDebugGUIClass);

        QMetaObject::connectSlotsByName(EconomicEngineDebugGUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *EconomicEngineDebugGUIClass)
    {
        EconomicEngineDebugGUIClass->setWindowTitle(QCoreApplication::translate("EconomicEngineDebugGUIClass", "EconomicEngineDebugGUI", nullptr));
        labSpeed->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Speed: ", nullptr));
        labZomXAxis->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Zoom: ", nullptr));
        labStep->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Step: ", nullptr));
        radBRealTime->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Real Time", nullptr));
        radStepByStep->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Step by step", nullptr));
        pBStart->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Start", nullptr));
        pBReset->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Reset", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EconomicEngineDebugGUIClass: public Ui_EconomicEngineDebugGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ECONOMICENGINEDEBUGGUI_H
