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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_EconomicEngineDebugGUIClass
{
public:
    QWidget *centralWidget;
    QCustomPlot *w_Plot;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *EconomicEngineDebugGUIClass)
    {
        if (EconomicEngineDebugGUIClass->objectName().isEmpty())
            EconomicEngineDebugGUIClass->setObjectName(QString::fromUtf8("EconomicEngineDebugGUIClass"));
        EconomicEngineDebugGUIClass->resize(600, 400);
        centralWidget = new QWidget(EconomicEngineDebugGUIClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        w_Plot = new QCustomPlot(centralWidget);
        w_Plot->setObjectName(QString::fromUtf8("w_Plot"));
        w_Plot->setGeometry(QRect(30, 20, 551, 291));
        EconomicEngineDebugGUIClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(EconomicEngineDebugGUIClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 26));
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
    } // retranslateUi

};

namespace Ui {
    class EconomicEngineDebugGUIClass: public Ui_EconomicEngineDebugGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ECONOMICENGINEDEBUGGUI_H
