#include "EconomicEngineDebugGUI.h"
#include "qcustomplot.h"
#include "TurnManager.h"
#include <QCloseEvent>
#include <thread>
#include "GraphManager.h"
#include "TraderManager.h"


EconomicEngineDebugGui::EconomicEngineDebugGui(QWidget* parent)
	: QMainWindow(parent)
{
	turnManager = TurnManager::getInstance();
	turnManager->addObserver(this);
	economicEngineThread = std::thread([](TurnManager* turnManager)-> int
	{
		turnManager->init();
		return turnManager->exec();
	}, turnManager);

	ui.setupUi(this);

	std::vector<GraphManager*> arrayCheckBox;
	arrayCheckBox.reserve(20);

	arrayCheckBox.push_back(ui.chBxGraphWheat);

	arrayCheckBox.push_back(ui.chBxGraphMeat);
	arrayCheckBox.push_back(ui.chBxGraphWood);
	arrayCheckBox.push_back(ui.chBxGraphLeather);
	arrayCheckBox.push_back(ui.chBxGraphCoal);
	arrayCheckBox.push_back(ui.chBxGraphBread);
	arrayCheckBox.push_back(ui.chBxGraphSteak);
	arrayCheckBox.push_back(ui.chBxGraphIronOre);
	arrayCheckBox.push_back(ui.chBxGraphIronArmor);
	arrayCheckBox.push_back(ui.chBxGraphWoodPickaxe);
	arrayCheckBox.push_back(ui.chBxGraphBow);
	arrayCheckBox.push_back(ui.chBxGraphIronAxe);
	arrayCheckBox.push_back(ui.chBxGraphIronHoe);
	arrayCheckBox.push_back(ui.chBxGraphIronPickaxe);
	arrayCheckBox.push_back(ui.chBxGraphIronSword);

	for (auto i = 0; arrayCheckBox.size() > i; ++i)
	{
		if (arrayCheckBox[i] != nullptr)
		{
			std::mt19937 randomEngine(i);
			const std::uniform_int_distribution<int> uniformDist(0, 255);

			const auto r = uniformDist(randomEngine);
			const auto g = uniformDist(randomEngine);
			const auto b = uniformDist(randomEngine);

			auto style = QString(
				"color: rgb(" + QString::number(r) + "," + QString::number(g) + "," + QString::number(b) +
				");");
			arrayCheckBox[i]->setStyleSheet(style);
			arrayCheckBox[i]->setGraphIndex(i);
			ui.customPlot->addGraph();
			ui.customPlot->graph(i)->setPen(QPen(QColor(r, g, b)));
			connect(arrayCheckBox[i], SIGNAL(clicked()), this,
			        SLOT(setGraphVisibility()));
		}
	}
	this->arrayCheckBox = arrayCheckBox;

	this->zoomXAxis = ui.horSlidZoomXAxis->value();
	connect(ui.horSlidZoomXAxis,SIGNAL(valueChanged(int)), this,SLOT(setZoomXAxis(int)));

	turnManager->setTurnSecond(ui.horSlidSpeed->value());
	connect(ui.horSlidSpeed, SIGNAL(valueChanged(int)), this, SLOT(setSpeed(int)));


	//QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
	//timeTicker->setTimeFormat("%h:%m:%s");
	//ui.customPlot->xAxis->setTicker(timeTicker);
	//ui.customPlot->axisRect()->setupFullAxesBox();
	//ui.customPlot->yAxis->setRange(0, 5);

	// make left and bottom axes transfer their ranges to right and top axes:
	connect(ui.customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui.customPlot->xAxis2, SLOT(setRange(QCPRange)));
	connect(ui.customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui.customPlot->yAxis2, SLOT(setRange(QCPRange)));

	connect(this, SIGNAL(nextTurn()), this, SLOT(realtimeDataSlot()));
}

EconomicEngineDebugGui::~EconomicEngineDebugGui()
{
	TurnManager::destroyInstance();
	this->turnManager = nullptr;
}

void EconomicEngineDebugGui::notify()
{
	this->nextTurn();
}

void EconomicEngineDebugGui::setGraphVisibility() const
{
	const auto checkBox = qobject_cast<GraphManager*>(QObject::sender());
	ui.customPlot->graph(checkBox->getGraphIndex())->setVisible(checkBox->isChecked());
}

void EconomicEngineDebugGui::setZoomXAxis(const int value)
{
	this->zoomXAxis = value;
	const auto key = turnManager->getTurnNumber();
	ui.customPlot->xAxis->setRange(key, this->zoomXAxis, Qt::AlignRight);
	ui.customPlot->replot();
}

void EconomicEngineDebugGui::setSpeed(const int value) const
{
	turnManager->setTurnSecond(value);
}

void EconomicEngineDebugGui::realtimeDataSlot() const
{
	static auto time(QTime::currentTime().elapsed() / 1000.0);
	// calculate two new data points:
	const auto key = turnManager->getTurnNumber();
	auto totalData = 0;
	for (auto checkBox : this->arrayCheckBox)
	{
		// add data to lines:
		ui.customPlot->graph(checkBox->getGraphIndex())->addData(
			key, qSin(10000000 + key) + qrand() / static_cast<double>(RAND_MAX) * 1 * qSin(
				checkBox->getGraphIndex() + (10000000 + key) / 0.3843));

		// rescale value (vertical) axis to fit the current data:
		ui.customPlot->graph(checkBox->getGraphIndex())->rescaleValueAxis(false);
		totalData += ui.customPlot->graph(checkBox->getGraphIndex())->data()->size();
	}

	ui.customPlot->xAxis->setRange(key, this->zoomXAxis, Qt::AlignRight);
	ui.customPlot->replot();

	// calculate frames per second:


	ui.statusBar->showMessage(
		QString("Total Data points: %1").arg(totalData), 0);
}

void EconomicEngineDebugGui::closeEvent(QCloseEvent* event)
{
	this->turnManager->stop();
	this->economicEngineThread.join();
}
