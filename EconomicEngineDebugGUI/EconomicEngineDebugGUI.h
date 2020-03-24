#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_EconomicEngineDebugGUI.h"

class EconomicEngineDebugGUI : public QMainWindow
{
	Q_OBJECT

public:
	explicit EconomicEngineDebugGUI(QWidget *parent = Q_NULLPTR);

	void addPoint(double x, double y);
	void clearData();
	void plot();
	
public slots:
	void on_btn_add_clicked();
	
private:
	Ui::EconomicEngineDebugGUIClass ui;
	QVector<double> qv_x, qv_y;

};
