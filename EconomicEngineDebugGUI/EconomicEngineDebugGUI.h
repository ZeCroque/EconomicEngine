#pragma once

#include <QtWidgets/QMainWindow>

#include "GraphManager.h"
#include "ui_EconomicEngineDebugGUI.h"

class EconomicEngineDebugGui final : public QMainWindow
{
Q_OBJECT

public:
	EconomicEngineDebugGui(QWidget* parent = Q_NULLPTR);

	~EconomicEngineDebugGui() = default;


public Q_SLOTS:
	void realtimeDataSlot() const;
	void setGraphVisibility() const;
	void setZoomXAxis(int value);

private:
	Ui::EconomicEngineDebugGUIClass ui;
	std::vector<GraphManager*> arrayCheckBox;
	QTimer dataTimer;

	int zoomXAxis;
};
