#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_EconomicEngineDebugGUI.h"
#include "Observer.h"
#include "TurnManager.h"

class EconomicEngineDebugGui final : public QMainWindow, public IObserver
{
Q_OBJECT

public:
	explicit EconomicEngineDebugGui(QWidget* parent = Q_NULLPTR);
	~EconomicEngineDebugGui();
	void notify() override;
	
	std::thread economicEngineThread;
	void EconomicEngineDebugGui::closeEvent(QCloseEvent* event) override;

public slots:
	void realtimeDataSlot() const;

private:
	Ui::EconomicEngineDebugGUIClass ui;
	QTimer dataTimer;
	TurnManager* turnManager;

signals:
	void nextTurn();

		
	
};
