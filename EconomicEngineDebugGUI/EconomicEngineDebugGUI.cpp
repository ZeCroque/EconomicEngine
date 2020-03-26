#include "EconomicEngineDebugGUI.h"
#include "qcustomplot.h"
#include "TurnManager.h"
#include <QCloseEvent>
#include <thread>

EconomicEngineDebugGui::EconomicEngineDebugGui(QWidget* parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	ui.w_Plot->addGraph(); // blue line
	ui.w_Plot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
	ui.w_Plot->addGraph(); // red line
	ui.w_Plot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

	QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
	timeTicker->setTimeFormat("%h:%m:%s");
	ui.w_Plot->xAxis->setTicker(timeTicker);
	ui.w_Plot->axisRect()->setupFullAxesBox();
	ui.w_Plot->yAxis->setRange(-1.2, 1.2);

	// make left and bottom axes transfer their ranges to right and top axes:
	connect(ui.w_Plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui.w_Plot->xAxis2, SLOT(setRange(QCPRange)));
	connect(ui.w_Plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui.w_Plot->yAxis2, SLOT(setRange(QCPRange)));

	// setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
	connect(this, SIGNAL(nextTurn()), this, SLOT(realtimeDataSlot()));

	//dataTimer.start(0); // Interval 0 means to refresh as fast as possible*/
}

void EconomicEngineDebugGui::notify()
{
	this->nextTurn();
}

void EconomicEngineDebugGui::realtimeDataSlot() const
{
	static QTime time(QTime::currentTime());
	// calculate two new data points:
	const double key = time.elapsed() / 1000.0; // time elapsed since start of demo, in seconds
	static double lastPointKey = 0;
	if (key - lastPointKey > 0.002) // at most add point every 2 ms
	{
		// add data to lines:
		ui.w_Plot->graph(0)->addData(key, qSin(key) + qrand() / static_cast<double>(RAND_MAX) * 1 * qSin(key / 0.3843));
		ui.w_Plot->graph(1)->addData(
			key, qCos(key) + qrand() / static_cast<double>(RAND_MAX) * 0.5 * qSin(key / 0.4364));
		// rescale value (vertical) axis to fit the current data:
		//ui->customPlot->graph(0)->rescaleValueAxis();
		//ui->customPlot->graph(1)->rescaleValueAxis(true);
		lastPointKey = key;
	}
	// make key axis range scroll with the data (at a constant range size of 8):
	ui.w_Plot->xAxis->setRange(key, 8, Qt::AlignRight);
	ui.w_Plot->replot();

	// calculate frames per second:
	static double lastFpsKey;
	static int frameCount;
	++frameCount;
	if (key - lastFpsKey > 2) // average fps over 2 seconds
	{
		ui.statusBar->showMessage(
			QString("%1 FPS, Total Data points: %2")
			.arg(frameCount / (key - lastFpsKey), 0, 'f', 0)
			.arg(ui.w_Plot->graph(0)->data()->size() + ui.w_Plot->graph(1)->data()->size())
			, 0);
		lastFpsKey = key;
		frameCount = 0;
	}
}

void EconomicEngineDebugGui::closeEvent(QCloseEvent* event)
{
	this->turnManager->stop();
	this->economicEngineThread->join();

	
}
