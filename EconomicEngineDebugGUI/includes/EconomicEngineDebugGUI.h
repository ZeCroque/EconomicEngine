#ifndef ECONOMIC_ENGINE_DEBUG_GUI
#define ECONOMIC_ENGINE_DEBUG_GUI
#include "ui_EconomicEngineDebugGUI.h"

#ifndef STANDALONE_MODE
#include <Signal.h>
#endif

class GraphManager; 
class JobManager;

class EconomicEngineDebugGui final : public QMainWindow
{
Q_OBJECT


//QT
//==========
public:
	explicit EconomicEngineDebugGui(QWidget* parent = nullptr);

	void showEvent(QShowEvent* event) override;
	void closeEvent(QCloseEvent* event) override;

public Q_SLOTS:
	void updateUiSlot();
	void setGraphVisibility();
	void setZoomXAxis(int value);
	void setSpeed(int value);
	void setYRange();
	void setXRange() const;
	void useXSlider(int);
	void toggleStart();
	void doKill();
	void doAdd();
	void doReset();
	void doInit();
	void updateUiJobs();

signals:
	void nextTurn();

private:
	Ui::EconomicEngineDebugGUIClass ui{};
	std::vector<GraphManager*> arrayCheckBox;
	std::vector<JobManager*> arrayJobs;
	int zoomXAxis;

//ECONOMIC ENGINE
//==========
private:
	int asksResolutionCount;

#ifdef STANDALONE_MODE
private:
	std::thread economicEngineThread;
	bool isRunning;
	bool hasEverRun;
	float speedFactor;
#else
public:
	const Signal<>& getInitializedSignal() const;
private:
	Signal<> initializedSignal;
#endif
};

#endif
