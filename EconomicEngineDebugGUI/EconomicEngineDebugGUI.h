#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_EconomicEngineDebugGUI.h"

class EconomicEngineDebugGUI final : public QMainWindow
{
Q_OBJECT

public:
	EconomicEngineDebugGUI(QWidget* parent = Q_NULLPTR);
	~EconomicEngineDebugGUI() = default;

public slots:
	void realtimeDataSlot() const;

private:
	Ui::EconomicEngineDebugGUIClass ui;
	QTimer dataTimer;
};
