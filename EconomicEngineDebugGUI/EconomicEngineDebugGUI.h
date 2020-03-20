#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_EconomicEngineDebugGUI.h"

class EconomicEngineDebugGUI : public QMainWindow
{
	Q_OBJECT

public:
	EconomicEngineDebugGUI(QWidget *parent = Q_NULLPTR);

private:
	Ui::EconomicEngineDebugGUIClass ui;
};
