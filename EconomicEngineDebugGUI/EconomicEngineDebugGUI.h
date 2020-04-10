#pragma once

#include <QtWidgets/QMainWindow>

#include "GraphManager.h"
#include "ui_EconomicEngineDebugGUI.h"
#include "Observer.h"
#include "TurnManager.h"

class EconomicEngineDebugGui final : public QMainWindow, public IObserver
{
Q_OBJECT

public:
	EconomicEngineDebugGui(QWidget* parent = Q_NULLPTR);
	~EconomicEngineDebugGui();
	void notify(Observable* sender) override;
	void EconomicEngineDebugGui::closeEvent(QCloseEvent* event) override;

public Q_SLOTS:
	void realtimeDataSlot();
	void setGraphVisibility() const;
	void setZoomXAxis(int value);
	void setSpeed(int value) const;
	void setStep(int value) const;
	void toggleStart() const;
	void setMode() const;

private:
	Ui::EconomicEngineDebugGUIClass ui;
	std::vector<GraphManager*> arrayCheckBox;
	QTimer dataTimer;
	std::thread economicEngineThread;
	TurnManager* turnManager;
	int zoomXAxis;
    bool realTime;

signals:
void nextTurn();
};
