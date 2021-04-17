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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_EconomicEngineDebugGUIClass
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
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
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *sBTraderNumber;
    QPushButton *pBReset;
    QVBoxLayout *layPlot;
    QCustomPlot *customPlot;
    QSlider *horSlidXNav;
    QFrame *line_3;
    QHBoxLayout *horizontalLayout_3;
    QGridLayout *gridLayJobs;
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_2;
    QPushButton *pBKill;
    QPushButton *pBAdd;
    QSpinBox *sBAdd;
    QSpinBox *sBKill;
    QComboBox *cBKill;
    QSpacerItem *horizontalSpacer_2;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *EconomicEngineDebugGUIClass)
    {
        if (EconomicEngineDebugGUIClass->objectName().isEmpty())
            EconomicEngineDebugGUIClass->setObjectName(QString::fromUtf8("EconomicEngineDebugGUIClass"));
        EconomicEngineDebugGUIClass->resize(1200, 610);
        EconomicEngineDebugGUIClass->setMinimumSize(QSize(1180, 610));
        EconomicEngineDebugGUIClass->setMaximumSize(QSize(1200, 610));
        EconomicEngineDebugGUIClass->setAutoFillBackground(false);
        EconomicEngineDebugGUIClass->setAnimated(true);
        centralWidget = new QWidget(EconomicEngineDebugGUIClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 1181, 571));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        layGraph = new QHBoxLayout();
        layGraph->setSpacing(6);
        layGraph->setObjectName(QString::fromUtf8("layGraph"));
        layGraphParam = new QVBoxLayout();
        layGraphParam->setSpacing(6);
        layGraphParam->setObjectName(QString::fromUtf8("layGraphParam"));
        layChBx = new QGridLayout();
        layChBx->setSpacing(6);
        layChBx->setObjectName(QString::fromUtf8("layChBx"));

        layGraphParam->addLayout(layChBx);

        line = new QFrame(verticalLayoutWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        layGraphParam->addWidget(line);

        LaySliders = new QGridLayout();
        LaySliders->setSpacing(6);
        LaySliders->setObjectName(QString::fromUtf8("LaySliders"));
        labSpeed = new QLabel(verticalLayoutWidget);
        labSpeed->setObjectName(QString::fromUtf8("labSpeed"));

        LaySliders->addWidget(labSpeed, 1, 0, 1, 1);

        horSlidSpeed = new QSlider(verticalLayoutWidget);
        horSlidSpeed->setObjectName(QString::fromUtf8("horSlidSpeed"));
        horSlidSpeed->setCursor(QCursor(Qt::PointingHandCursor));
        horSlidSpeed->setMinimum(1);
        horSlidSpeed->setMaximum(50);
        horSlidSpeed->setValue(5);
        horSlidSpeed->setOrientation(Qt::Horizontal);
        horSlidSpeed->setInvertedAppearance(false);
        horSlidSpeed->setInvertedControls(false);

        LaySliders->addWidget(horSlidSpeed, 1, 1, 1, 1);

        horSlidZoomXAxis = new QSlider(verticalLayoutWidget);
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

        labZomXAxis = new QLabel(verticalLayoutWidget);
        labZomXAxis->setObjectName(QString::fromUtf8("labZomXAxis"));

        LaySliders->addWidget(labZomXAxis, 0, 0, 1, 1);

        labStep = new QLabel(verticalLayoutWidget);
        labStep->setObjectName(QString::fromUtf8("labStep"));

        LaySliders->addWidget(labStep, 2, 0, 1, 1);

        horSlidStep = new QSlider(verticalLayoutWidget);
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

        line_2 = new QFrame(verticalLayoutWidget);
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
        radBRealTime = new QRadioButton(verticalLayoutWidget);
        radBRealTime->setObjectName(QString::fromUtf8("radBRealTime"));
        radBRealTime->setLayoutDirection(Qt::LeftToRight);
        radBRealTime->setAutoFillBackground(false);
        radBRealTime->setChecked(true);

        layRadButton->addWidget(radBRealTime);

        radStepByStep = new QRadioButton(verticalLayoutWidget);
        radStepByStep->setObjectName(QString::fromUtf8("radStepByStep"));

        layRadButton->addWidget(radStepByStep);


        layButton->addLayout(layRadButton);

        LayPushButton = new QVBoxLayout();
        LayPushButton->setSpacing(6);
        LayPushButton->setObjectName(QString::fromUtf8("LayPushButton"));
        pBStart = new QPushButton(verticalLayoutWidget);
        pBStart->setObjectName(QString::fromUtf8("pBStart"));
        pBStart->setCheckable(true);

        LayPushButton->addWidget(pBStart);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        sBTraderNumber = new QSpinBox(verticalLayoutWidget);
        sBTraderNumber->setObjectName(QString::fromUtf8("sBTraderNumber"));
        sBTraderNumber->setMinimum(0);
        sBTraderNumber->setMaximum(1000);
        sBTraderNumber->setSingleStep(10);
        sBTraderNumber->setValue(100);

        horizontalLayout->addWidget(sBTraderNumber);


        LayPushButton->addLayout(horizontalLayout);

        pBReset = new QPushButton(verticalLayoutWidget);
        pBReset->setObjectName(QString::fromUtf8("pBReset"));
        pBReset->setCheckable(true);

        LayPushButton->addWidget(pBReset);


        layButton->addLayout(LayPushButton);


        layGraphParam->addLayout(layButton);


        layGraph->addLayout(layGraphParam);

        layPlot = new QVBoxLayout();
        layPlot->setSpacing(6);
        layPlot->setObjectName(QString::fromUtf8("layPlot"));
        customPlot = new QCustomPlot(verticalLayoutWidget);
        customPlot->setObjectName(QString::fromUtf8("customPlot"));
        customPlot->setMinimumSize(QSize(830, 300));
        customPlot->setMaximumSize(QSize(830, 300));

        layPlot->addWidget(customPlot);

        horSlidXNav = new QSlider(verticalLayoutWidget);
        horSlidXNav->setObjectName(QString::fromUtf8("horSlidXNav"));
        horSlidXNav->setValue(99);
        horSlidXNav->setOrientation(Qt::Horizontal);

        layPlot->addWidget(horSlidXNav);


        layGraph->addLayout(layPlot);


        verticalLayout->addLayout(layGraph);

        line_3 = new QFrame(verticalLayoutWidget);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_3);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        gridLayJobs = new QGridLayout();
        gridLayJobs->setSpacing(6);
        gridLayJobs->setObjectName(QString::fromUtf8("gridLayJobs"));
        gridLayJobs->setSizeConstraint(QLayout::SetMinimumSize);

        horizontalLayout_3->addLayout(gridLayJobs);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        pBKill = new QPushButton(verticalLayoutWidget);
        pBKill->setObjectName(QString::fromUtf8("pBKill"));

        gridLayout_2->addWidget(pBKill, 1, 1, 1, 1);

        pBAdd = new QPushButton(verticalLayoutWidget);
        pBAdd->setObjectName(QString::fromUtf8("pBAdd"));

        gridLayout_2->addWidget(pBAdd, 0, 1, 1, 1);

        sBAdd = new QSpinBox(verticalLayoutWidget);
        sBAdd->setObjectName(QString::fromUtf8("sBAdd"));
        sBAdd->setMinimum(1);

        gridLayout_2->addWidget(sBAdd, 0, 0, 1, 1);

        sBKill = new QSpinBox(verticalLayoutWidget);
        sBKill->setObjectName(QString::fromUtf8("sBKill"));
        sBKill->setMinimum(1);

        gridLayout_2->addWidget(sBKill, 1, 0, 1, 1);


        gridLayout->addLayout(gridLayout_2, 0, 1, 1, 1);

        cBKill = new QComboBox(verticalLayoutWidget);
        cBKill->setObjectName(QString::fromUtf8("cBKill"));

        gridLayout->addWidget(cBKill, 0, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 2, 1, 1);


        horizontalLayout_3->addLayout(gridLayout);


        verticalLayout->addLayout(horizontalLayout_3);

        EconomicEngineDebugGUIClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(EconomicEngineDebugGUIClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        statusBar->setEnabled(true);
        EconomicEngineDebugGUIClass->setStatusBar(statusBar);
        QWidget::setTabOrder(horSlidZoomXAxis, horSlidSpeed);
        QWidget::setTabOrder(horSlidSpeed, horSlidStep);
        QWidget::setTabOrder(horSlidStep, radBRealTime);
        QWidget::setTabOrder(radBRealTime, radStepByStep);
        QWidget::setTabOrder(radStepByStep, pBStart);
        QWidget::setTabOrder(pBStart, pBReset);
        QWidget::setTabOrder(pBReset, horSlidXNav);
        QWidget::setTabOrder(horSlidXNav, cBKill);

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
        label->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "New trader nb.:", nullptr));
        pBReset->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Reset", nullptr));
        pBKill->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Kill", nullptr));
        pBAdd->setText(QCoreApplication::translate("EconomicEngineDebugGUIClass", "Add", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EconomicEngineDebugGUIClass: public Ui_EconomicEngineDebugGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ECONOMICENGINEDEBUGGUI_H