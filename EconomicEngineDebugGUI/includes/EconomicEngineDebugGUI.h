#pragma once
#include <Signal.h>

class GraphManager; 
class JobManager;
#include "ui_EconomicEngineDebugGUI.h"

class EconomicEngineDebugGui final : public QMainWindow
{
Q_OBJECT

public:
	explicit EconomicEngineDebugGui(QWidget* parent = Q_NULLPTR);
	~EconomicEngineDebugGui() = default;

	const Signal<>& getInitializedSignal() const;

	void showEvent(QShowEvent* event) override;
	void closeEvent(QCloseEvent* event) override;

public Q_SLOTS:
	void updateUiSlot();
	void setGraphVisibility();
	void setZoomXAxis(int value);
	void setSpeed(int value) const;
	void setYRange();
	void setXRange() const;
	void useXSlider(int);
	void toggleStart() const;
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
	int zoomXAxis;
	int asksResolutionCount;
	
	Signal<> initializedSignal;

signals:
	void nextTurn();
};
