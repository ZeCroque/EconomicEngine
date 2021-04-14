#pragma once

#include <any>
#include <QtWidgets/QMainWindow>
#include "GraphManager.h"
#include "JobManager.h"
#include "EconomicEngine.h"
#include "Signal.h"
#include "ui_EconomicEngineDebugGUI.h"

class EconomicEngineDebugGui final : public QMainWindow, IObserver
{
Q_OBJECT

public:
	explicit EconomicEngineDebugGui(QWidget* parent = Q_NULLPTR);
	~EconomicEngineDebugGui();

	const Signal<std::any>& getCloseSignal() const;
	void notify(Observable* sender) override;
	void closeEvent(QCloseEvent* event) override;

public Q_SLOTS:
	void updateUiSlot();
	void setGraphVisibility();
	void setZoomXAxis(int value);
	void setSpeed(int value) const;
	void setStep(int value) const;
	void setYRange();
	void setXRange() const;
	void useXSlider(int);
	void toggleStart() const;
	void setMode() const;
	void doKill();
	void doAdd();
	void doReset();
	void doInit();
	void updateUiJobs();

private:
	Ui::EconomicEngineDebugGUIClass ui{};
	std::vector<GraphManager*> arrayCheckBox;
	std::vector<JobManager*> arrayJobs;
	std::thread economicEngineThread;
	EconomicEngine* turnManager;
	TraderManager* traderManager;
	Signal<std::any> closeSignal;
	int zoomXAxis;

signals:
	void nextTurn();
};
