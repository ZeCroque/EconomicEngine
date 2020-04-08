/********************************************************************************
** Form generated from reading UI file 'EconomicEngineDebugGUI.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ECONOMICENGINEDEBUGGUI_H
#define UI_ECONOMICENGINEDEBUGGUI_H

#include <GraphManager.h>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
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
    GraphManager *chBxGraph;
    GraphManager *chBxGraph_2;
    GraphManager *chBxGraph_3;
    GraphManager *chBxGraph_4;
    GraphManager *chBxGraph_5;
    GraphManager *chBxGraph_6;
    GraphManager *chBxGraph_7;
    GraphManager *chBxGraph_8;
    GraphManager *chBxGraph_9;
    GraphManager *chBxGraph_10;
    GraphManager *chBxGraph_11;
    GraphManager *chBxGraph_12;
    QGridLayout *LaySliders;
    QLabel *labSpeed;
    QSlider *horSlidZoomXAxis;
    QLabel *labZomXAxis;
    QSlider *horSlidSpeed;
    QSpacerItem *verticalSpacer;
    QCustomPlot *customPlot;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *EconomicEngineDebugGUIClass)
    {
        if (EconomicEngineDebugGUIClass->objectName().isEmpty())
            EconomicEngineDebugGUIClass->setObjectName(QString::fromUtf8("EconomicEngineDebugGUIClass"));
        EconomicEngineDebugGUIClass->resize(1180, 680);
        EconomicEngineDebugGUIClass->setMinimumSize(QSize(1180, 680));
        EconomicEngineDebugGUIClass->setMaximumSize(QSize(1180, 680));
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
        chBxGraph = new GraphManager(horizontalLayoutWidget_3);
        chBxGraph->setObjectName(QString::fromUtf8("chBxGraph"));
        chBxGraph->setEnabled(false);
        chBxGraph->setCursor(QCursor(Qt::PointingHandCursor));
        chBxGraph->setCheckable(false);
        chBxGraph->setChecked(false);
        chBxGraph->setProperty("graphIndex", QVariant(0));

        layChBx->addWidget(chBxGraph, 0, 0, 1, 1);

        chBxGraph_2 = new GraphManager(horizontalLayoutWidget_3);
        chBxGraph_2->setObjectName(QString::fromUtf8("chBxGraph_2"));
        chBxGraph_2->setEnabled(false);
        chBxGraph_2->setCursor(QCursor(Qt::PointingHandCursor));
        chBxGraph_2->setCheckable(false);
        chBxGraph_2->setChecked(false);
        chBxGraph_2->setProperty("graphIndex", QVariant(0));

        layChBx->addWidget(chBxGraph_2, 0, 1, 1, 1);

        chBxGraph_3 = new GraphManager(horizontalLayoutWidget_3);
        chBxGraph_3->setObjectName(QString::fromUtf8("chBxGraph_3"));
        chBxGraph_3->setEnabled(false);
        chBxGraph_3->setCursor(QCursor(Qt::PointingHandCursor));
        chBxGraph_3->setCheckable(false);
        chBxGraph_3->setChecked(false);
        chBxGraph_3->setProperty("graphIndex", QVariant(0));

        layChBx->addWidget(chBxGraph_3, 0, 2, 1, 1);

        chBxGraph_4 = new GraphManager(horizontalLayoutWidget_3);
        chBxGraph_4->setObjectName(QString::fromUtf8("chBxGraph_4"));
        chBxGraph_4->setEnabled(false);
        chBxGraph_4->setCursor(QCursor(Qt::PointingHandCursor));
        chBxGraph_4->setCheckable(false);
        chBxGraph_4->setChecked(false);
        chBxGraph_4->setProperty("graphIndex", QVariant(0));

        layChBx->addWidget(chBxGraph_4, 1, 0, 1, 1);

        chBxGraph_5 = new GraphManager(horizontalLayoutWidget_3);
        chBxGraph_5->setObjectName(QString::fromUtf8("chBxGraph_5"));
        chBxGraph_5->setEnabled(false);
        chBxGraph_5->setCursor(QCursor(Qt::PointingHandCursor));
        chBxGraph_5->setCheckable(false);
        chBxGraph_5->setChecked(false);
        chBxGraph_5->setProperty("graphIndex", QVariant(0));

        layChBx->addWidget(chBxGraph_5, 1, 1, 1, 1);

        chBxGraph_6 = new GraphManager(horizontalLayoutWidget_3);
        chBxGraph_6->setObjectName(QString::fromUtf8("chBxGraph_6"));
        chBxGraph_6->setEnabled(false);
        chBxGraph_6->setCursor(QCursor(Qt::PointingHandCursor));
        chBxGraph_6->setCheckable(false);
        chBxGraph_6->setChecked(false);
        chBxGraph_6->setProperty("graphIndex", QVariant(0));

        layChBx->addWidget(chBxGraph_6, 1, 2, 1, 1);

        chBxGraph_7 = new GraphManager(horizontalLayoutWidget_3);
        chBxGraph_7->setObjectName(QString::fromUtf8("chBxGraph_7"));
        chBxGraph_7->setEnabled(false);
        chBxGraph_7->setCursor(QCursor(Qt::PointingHandCursor));
        chBxGraph_7->setCheckable(false);
        chBxGraph_7->setChecked(false);
        chBxGraph_7->setProperty("graphIndex", QVariant(0));

        layChBx->addWidget(chBxGraph_7, 2, 0, 1, 1);

        chBxGraph_8 = new GraphManager(horizontalLayoutWidget_3);
        chBxGraph_8->setObjectName(QString::fromUtf8("chBxGraph_8"));
        chBxGraph_8->setEnabled(false);
        chBxGraph_8->setCursor(QCursor(Qt::PointingHandCursor));
        chBxGraph_8->setCheckable(false);
        chBxGraph_8->setChecked(false);
        chBxGraph_8->setProperty("graphIndex", QVariant(0));

        layChBx->addWidget(chBxGraph_8, 2, 1, 1, 1);

        chBxGraph_9 = new GraphManager(horizontalLayoutWidget_3);
        chBxGraph_9->setObjectName(QString::fromUtf8("chBxGraph_9"));
        chBxGraph_9->setEnabled(false);
        chBxGraph_9->setCursor(QCursor(Qt::PointingHandCursor));
        chBxGraph_9->setCheckable(false);
        chBxGraph_9->setChecked(false);
        chBxGraph_9->setProperty("graphIndex", QVariant(0));

        layChBx->addWidget(chBxGraph_9, 2, 2, 1, 1);

        chBxGraph_10 = new GraphManager(horizontalLayoutWidget_3);
        chBxGraph_10->setObjectName(QString::fromUtf8("chBxGraph_10"));
        chBxGraph_10->setEnabled(false);
        chBxGraph_10->setCursor(QCursor(Qt::PointingHandCursor));
        chBxGraph_10->setCheckable(false);
        chBxGraph_10->setChecked(false);
        chBxGraph_10->setProperty("graphIndex", QVariant(0));

        layChBx->addWidget(chBxGraph_10, 3, 0, 1, 1);

        chBxGraph_11 = new GraphManager(horizontalLayoutWidget_3);
        chBxGraph_11->setObjectName(QString::fromUtf8("chBxGraph_11"));
        chBxGraph_11->setEnabled(false);
        chBxGraph_11->setCursor(QCursor(Qt::PointingHandCursor));
        chBxGraph_11->setCheckable(false);
        chBxGraph_11->setChecked(false);
        chBxGraph_11->setProperty("graphIndex", QVariant(0));

        layChBx->addWidget(chBxGraph_11, 3, 1, 1, 1);

        chBxGraph_12 = new GraphManager(horizontalLayoutWidget_3);
        chBxGraph_12->setObjectName(QString::fromUtf8("chBxGraph_12"));
        chBxGraph_12->setEnabled(false);
        chBxGraph_12->setCursor(QCursor(Qt::PointingHandCursor));
        chBxGraph_12->setCheckable(false);
        chBxGraph_12->setChecked(false);
        chBxGraph_12->setProperty("graphIndex", QVariant(0));

        layChBx->addWidget(chBxGraph_12, 3, 2, 1, 1);


        layGraphParam->addLayout(layChBx);

        LaySliders = new QGridLayout();
        LaySliders->setSpacing(6);
        LaySliders->setObjectName(QString::fromUtf8("LaySliders"));
        labSpeed = new QLabel(horizontalLayoutWidget_3);
        labSpeed->setObjectName(QString::fromUtf8("labSpeed"));

        LaySliders->addWidget(labSpeed, 1, 0, 1, 1);

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


        layGraphParam->addLayout(LaySliders);

        verticalSpacer = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Minimum);

        layGraphParam->addItem(verticalSpacer);


        layGraph->addLayout(layGraphParam);

        customPlot = new QCustomPlot(horizontalLayoutWidget_3);
        customPlot->setObjectName(QString::fromUtf8("customPlot"));
        customPlot->setMinimumSize(QSize(830, 300));
        customPlot->setMaximumSize(QSize(830, 300));

        layGraph->addWidget(customPlot);

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
        chBxGraph->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Empty", nullptr));
        chBxGraph_2->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Empty", nullptr));
        chBxGraph_3->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Empty", nullptr));
        chBxGraph_4->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Empty", nullptr));
        chBxGraph_5->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Empty", nullptr));
        chBxGraph_6->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Empty", nullptr));
        chBxGraph_7->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Empty", nullptr));
        chBxGraph_8->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Empty", nullptr));
        chBxGraph_9->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Empty", nullptr));
        chBxGraph_10->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Empty", nullptr));
        chBxGraph_11->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Empty", nullptr));
        chBxGraph_12->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Empty", nullptr));
        labSpeed->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Speed: ", nullptr));
        labZomXAxis->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Zoom: ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EconomicEngineDebugGUIClass: public Ui_EconomicEngineDebugGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ECONOMICENGINEDEBUGGUI_H
