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
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
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
    QHBoxLayout *layGraphChBx;
    QVBoxLayout *LayOne;
    GraphManager *chBxGraphWheat;
    GraphManager *chBxGraphMeat;
    GraphManager *chBxGraphWood;
    GraphManager *chBxGraphLeather;
    GraphManager *chBxGraphCoal;
    QVBoxLayout *LayTwo;
    GraphManager *chBxGraphBread;
    GraphManager *chBxGraphSteak;
    GraphManager *chBxGraphIronOre;
    GraphManager *chBxGraphIronArmor;
    GraphManager *chBxGraphWoodPickaxe;
    QVBoxLayout *LayThree;
    GraphManager *chBxGraphBow;
    GraphManager *chBxGraphIronAxe;
    GraphManager *chBxGraphIronHoe;
    GraphManager *chBxGraphIronPickaxe;
    GraphManager *chBxGraphIronSword;
    QGridLayout *LaySliders;
    QLabel *labZomXAxis;
    QSlider *horSlidZoomXAxis;
    QLabel *labSpeed;
    QSlider *horSlidSpeed;
    QSpacerItem *verticalSpacer;
    QCustomPlot *customPlot;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *EconomicEngineDebugGUIClass)
    {
        if (EconomicEngineDebugGUIClass->objectName().isEmpty())
            EconomicEngineDebugGUIClass->setObjectName(QString::fromUtf8("EconomicEngineDebugGUIClass"));
        EconomicEngineDebugGUIClass->resize(1177, 689);
        centralWidget = new QWidget(EconomicEngineDebugGUIClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayoutWidget_3 = new QWidget(centralWidget);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(10, 10, 1151, 341));
        layGraph = new QHBoxLayout(horizontalLayoutWidget_3);
        layGraph->setSpacing(6);
        layGraph->setContentsMargins(11, 11, 11, 11);
        layGraph->setObjectName(QString::fromUtf8("layGraph"));
        layGraph->setContentsMargins(0, 0, 0, 0);
        layGraphParam = new QVBoxLayout();
        layGraphParam->setSpacing(6);
        layGraphParam->setObjectName(QString::fromUtf8("layGraphParam"));
        layGraphChBx = new QHBoxLayout();
        layGraphChBx->setSpacing(6);
        layGraphChBx->setObjectName(QString::fromUtf8("layGraphChBx"));
        LayOne = new QVBoxLayout();
        LayOne->setSpacing(6);
        LayOne->setObjectName(QString::fromUtf8("LayOne"));
        chBxGraphWheat = new GraphManager(horizontalLayoutWidget_3);
        chBxGraphWheat->setObjectName(QString::fromUtf8("chBxGraphWheat"));
        chBxGraphWheat->setChecked(true);
        chBxGraphWheat->setProperty("graphIndex", QVariant(0));

        LayOne->addWidget(chBxGraphWheat);

        chBxGraphMeat = new GraphManager(horizontalLayoutWidget_3);
        chBxGraphMeat->setObjectName(QString::fromUtf8("chBxGraphMeat"));
        chBxGraphMeat->setChecked(true);

        LayOne->addWidget(chBxGraphMeat);

        chBxGraphWood = new GraphManager(horizontalLayoutWidget_3);
        chBxGraphWood->setObjectName(QString::fromUtf8("chBxGraphWood"));
        chBxGraphWood->setChecked(true);

        LayOne->addWidget(chBxGraphWood);

        chBxGraphLeather = new GraphManager(horizontalLayoutWidget_3);
        chBxGraphLeather->setObjectName(QString::fromUtf8("chBxGraphLeather"));
        chBxGraphLeather->setChecked(true);

        LayOne->addWidget(chBxGraphLeather);

        chBxGraphCoal = new GraphManager(horizontalLayoutWidget_3);
        chBxGraphCoal->setObjectName(QString::fromUtf8("chBxGraphCoal"));
        chBxGraphCoal->setChecked(true);

        LayOne->addWidget(chBxGraphCoal);


        layGraphChBx->addLayout(LayOne);

        LayTwo = new QVBoxLayout();
        LayTwo->setSpacing(6);
        LayTwo->setObjectName(QString::fromUtf8("LayTwo"));
        chBxGraphBread = new GraphManager(horizontalLayoutWidget_3);
        chBxGraphBread->setObjectName(QString::fromUtf8("chBxGraphBread"));
        chBxGraphBread->setChecked(true);

        LayTwo->addWidget(chBxGraphBread);

        chBxGraphSteak = new GraphManager(horizontalLayoutWidget_3);
        chBxGraphSteak->setObjectName(QString::fromUtf8("chBxGraphSteak"));
        chBxGraphSteak->setChecked(true);

        LayTwo->addWidget(chBxGraphSteak);

        chBxGraphIronOre = new GraphManager(horizontalLayoutWidget_3);
        chBxGraphIronOre->setObjectName(QString::fromUtf8("chBxGraphIronOre"));
        chBxGraphIronOre->setChecked(true);

        LayTwo->addWidget(chBxGraphIronOre);

        chBxGraphIronArmor = new GraphManager(horizontalLayoutWidget_3);
        chBxGraphIronArmor->setObjectName(QString::fromUtf8("chBxGraphIronArmor"));
        chBxGraphIronArmor->setChecked(true);

        LayTwo->addWidget(chBxGraphIronArmor);

        chBxGraphWoodPickaxe = new GraphManager(horizontalLayoutWidget_3);
        chBxGraphWoodPickaxe->setObjectName(QString::fromUtf8("chBxGraphWoodPickaxe"));
        chBxGraphWoodPickaxe->setChecked(true);

        LayTwo->addWidget(chBxGraphWoodPickaxe);


        layGraphChBx->addLayout(LayTwo);

        LayThree = new QVBoxLayout();
        LayThree->setSpacing(6);
        LayThree->setObjectName(QString::fromUtf8("LayThree"));
        chBxGraphBow = new GraphManager(horizontalLayoutWidget_3);
        chBxGraphBow->setObjectName(QString::fromUtf8("chBxGraphBow"));
        chBxGraphBow->setChecked(true);

        LayThree->addWidget(chBxGraphBow);

        chBxGraphIronAxe = new GraphManager(horizontalLayoutWidget_3);
        chBxGraphIronAxe->setObjectName(QString::fromUtf8("chBxGraphIronAxe"));
        chBxGraphIronAxe->setChecked(true);

        LayThree->addWidget(chBxGraphIronAxe);

        chBxGraphIronHoe = new GraphManager(horizontalLayoutWidget_3);
        chBxGraphIronHoe->setObjectName(QString::fromUtf8("chBxGraphIronHoe"));
        chBxGraphIronHoe->setChecked(true);

        LayThree->addWidget(chBxGraphIronHoe);

        chBxGraphIronPickaxe = new GraphManager(horizontalLayoutWidget_3);
        chBxGraphIronPickaxe->setObjectName(QString::fromUtf8("chBxGraphIronPickaxe"));
        chBxGraphIronPickaxe->setChecked(true);

        LayThree->addWidget(chBxGraphIronPickaxe);

        chBxGraphIronSword = new GraphManager(horizontalLayoutWidget_3);
        chBxGraphIronSword->setObjectName(QString::fromUtf8("chBxGraphIronSword"));
        chBxGraphIronSword->setChecked(true);

        LayThree->addWidget(chBxGraphIronSword);


        layGraphChBx->addLayout(LayThree);


        layGraphParam->addLayout(layGraphChBx);

        LaySliders = new QGridLayout();
        LaySliders->setSpacing(6);
        LaySliders->setObjectName(QString::fromUtf8("LaySliders"));
        labZomXAxis = new QLabel(horizontalLayoutWidget_3);
        labZomXAxis->setObjectName(QString::fromUtf8("labZomXAxis"));

        LaySliders->addWidget(labZomXAxis, 0, 0, 1, 1);

        horSlidZoomXAxis = new QSlider(horizontalLayoutWidget_3);
        horSlidZoomXAxis->setObjectName(QString::fromUtf8("horSlidZoomXAxis"));
        horSlidZoomXAxis->setMinimum(1);
        horSlidZoomXAxis->setMaximum(100);
        horSlidZoomXAxis->setSingleStep(1);
        horSlidZoomXAxis->setValue(10);
        horSlidZoomXAxis->setOrientation(Qt::Horizontal);
        horSlidZoomXAxis->setInvertedAppearance(true);
        horSlidZoomXAxis->setInvertedControls(false);

        LaySliders->addWidget(horSlidZoomXAxis, 0, 1, 1, 1);

        labSpeed = new QLabel(horizontalLayoutWidget_3);
        labSpeed->setObjectName(QString::fromUtf8("labSpeed"));

        LaySliders->addWidget(labSpeed, 1, 0, 1, 1);

        horSlidSpeed = new QSlider(horizontalLayoutWidget_3);
        horSlidSpeed->setObjectName(QString::fromUtf8("horSlidSpeed"));
        horSlidSpeed->setMinimum(1);
        horSlidSpeed->setMaximum(100);
        horSlidSpeed->setValue(10);
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
        menuBar = new QMenuBar(EconomicEngineDebugGUIClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1177, 26));
        EconomicEngineDebugGUIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(EconomicEngineDebugGUIClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        EconomicEngineDebugGUIClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(EconomicEngineDebugGUIClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        EconomicEngineDebugGUIClass->setStatusBar(statusBar);

        retranslateUi(EconomicEngineDebugGUIClass);

        QMetaObject::connectSlotsByName(EconomicEngineDebugGUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *EconomicEngineDebugGUIClass)
    {
        EconomicEngineDebugGUIClass->setWindowTitle(QCoreApplication::translate("EconomicEngineDebugGUIClass", "EconomicEngineDebugGUI", nullptr));
        chBxGraphWheat->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Wheat", nullptr));
        chBxGraphMeat->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Meat", nullptr));
        chBxGraphWood->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Wood", nullptr));
        chBxGraphLeather->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Leather", nullptr));
        chBxGraphCoal->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Coal", nullptr));
        chBxGraphBread->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Bread", nullptr));
        chBxGraphSteak->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Steak", nullptr));
        chBxGraphIronOre->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Iron Ore", nullptr));
        chBxGraphIronArmor->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Iron Armor", nullptr));
        chBxGraphWoodPickaxe->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Wood Pickaxe", nullptr));
        chBxGraphBow->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Bow", nullptr));
        chBxGraphIronAxe->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Iron Axe", nullptr));
        chBxGraphIronHoe->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Iron Hoe", nullptr));
        chBxGraphIronPickaxe->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Iron Pickaxe", nullptr));
        chBxGraphIronSword->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Iron Sword", nullptr));
        labZomXAxis->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Zoom X: ", nullptr));
        labSpeed->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Seed: ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EconomicEngineDebugGUIClass: public Ui_EconomicEngineDebugGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ECONOMICENGINEDEBUGGUI_H
