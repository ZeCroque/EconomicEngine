#ifndef ECONOMIC_ENGINE_DEBUG_GUI
#define ECONOMIC_ENGINE_DEBUG_GUI
#include "ui_EconomicEngineDebugGUI.h"

#ifndef STANDALONE_MODE
#include "Signal.h"
#endif

class GraphManager; 
class JobManager;

class EconomicEngineDebugGui final : public QMainWindow
{
Q_OBJECT


//QT
//==========
public:
	explicit EconomicEngineDebugGui(QWidget* inParent = nullptr);

	void showEvent(QShowEvent* inEvent) override;
	void closeEvent(QCloseEvent* inEvent) override;

public Q_SLOTS:
	void updateUiSlot();
	void setGraphVisibility();
	void setZoomXAxis(int inValue);
	void setSpeed(int inValue);
	void setYRange();
	void setXRange() const;
	void useXSlider(int inValue);
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
	bool bIsRunning;
	bool bHasBeenReset;
	bool bHasEverRun;
	float speedFactor;
#else
public:
	const Signal<>& getInitializedSignal() const;
private:
	Signal<> initializedSignal;
#endif
};

#endif
