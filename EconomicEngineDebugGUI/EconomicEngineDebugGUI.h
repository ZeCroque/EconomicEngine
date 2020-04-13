#pragma once

#include <QtWidgets/QMainWindow>

#include "GraphManager.h"
#include "JobManager.h"
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
	void updateUiSlot();
	void setGraphVisibility();
	void setZoomXAxis(int value);
	void setSpeed(int value) const;
	void setStep(int value) const;
	void setYRange();
	void setXRange() const;
	void useXSlider(int value);
	void toggleStart() const;
	void setMode() const;
	void doKill();
	void doReset();
	void doInit();
	void updateUiJobs();

private:
	Ui::EconomicEngineDebugGUIClass ui;
	std::vector<GraphManager*> arrayCheckBox;
	std::vector<JobManager*> arrayJobs;
	QTimer dataTimer;
	std::thread economicEngineThread;
	TurnManager* turnManager;
	TraderManager* traderManager;
	int zoomXAxis;
	bool realTime;

signals:
	void nextTurn();
};
