#include "EconomicEngineDebugGUI.h"
#include "qcustomplot.h"
#include "TurnManager.h"
#include <QCloseEvent>
#include <thread>
#include "GraphManager.h"
#include "TraderManager.h"
#include "TradableManager.h"


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

	const auto tradableManager = TradableManager::getInstance();
	auto itemsName = tradableManager->getTradablesName();
	auto itemsKeys = tradableManager->getKeys();

	auto row = 0;
	auto column = 0;
	for (auto i = 0; i < itemsName.size(); ++i)
	{
		if (itemsName.size() > i)
		{
			auto checkBox = new GraphManager(this);
			checkBox->setText(QString::fromStdString(itemsName[i]));
			checkBox->setItemId(itemsKeys[i]);
			checkBox->setEnabled(true);
			checkBox->setCheckable(true);
			checkBox->setChecked(true);

			std::mt19937 randomEngine(i);
			const std::uniform_int_distribution<int> uniformDist(0, 255);

			const auto r = uniformDist(randomEngine);
			const auto g = uniformDist(randomEngine);
			const auto b = uniformDist(randomEngine);

			auto style = QString(
				"color: rgb(" + QString::number(r) + "," + QString::number(g) + "," + QString::number(b) +
				");");

			checkBox->setStyleSheet(style);
			checkBox->setGraphIndex(i);
			ui.customPlot->addGraph();
			ui.customPlot->graph(i)->setPen(QPen(QColor(r, g, b)));
			connect(checkBox, SIGNAL(clicked()), this, SLOT(setGraphVisibility()));

			this->arrayCheckBox.push_back(checkBox);

			ui.layChBx->addWidget(checkBox, row, column);
			if (column == 2)
			{
				++row;
				column = 0;
			}
			else
			{
				++column;
			}
		}
	}

	this->zoomXAxis = ui.horSlidZoomXAxis->value();
	connect(ui.horSlidZoomXAxis,SIGNAL(valueChanged(int)), this,SLOT(setZoomXAxis(int)));

	turnManager->setTurnSecond(ui.horSlidSpeed->value());
	connect(ui.horSlidSpeed, SIGNAL(valueChanged(int)), this, SLOT(setSpeed(int)));

	turnManager->setStep(ui.horSlidStep->value());
	connect(ui.horSlidStep, SIGNAL(valueChanged(int)), this, SLOT(setStep(int)));


	//QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
	//timeTicker->setTimeFormat("%h:%m:%s");
	//ui.customPlot->xAxis->setTicker(timeTicker);
	//ui.customPlot->axisRect()->setupFullAxesBox();
	//ui.customPlot->yAxis->setRange(0, 5);

	// make left and bottom axes transfer their ranges to right and top axes:
	connect(ui.customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui.customPlot->xAxis2, SLOT(setRange(QCPRange)));
	connect(ui.customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui.customPlot->yAxis2, SLOT(setRange(QCPRange)));

	connect(this, SIGNAL(nextTurn()), this, SLOT(realtimeDataSlot()));

	connect(ui.pBStart, SIGNAL(clicked()), this,SLOT(toggleStart()));
}

EconomicEngineDebugGui::~EconomicEngineDebugGui()
{
	TurnManager::destroyInstance();
	this->turnManager = nullptr;
}

void EconomicEngineDebugGui::notify(Observable* sender)
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

void EconomicEngineDebugGui::setStep(const int value) const
{
	turnManager->setStep(value);
}

void EconomicEngineDebugGui::toggleStart() const
{
	if (ui.pBStart->isChecked())
	{
		ui.pBStart->setText("Stop");
	}
	else
	{
		ui.pBStart->setText("Start");
	}
	turnManager->setIsStarted(ui.pBStart->isChecked());
}

void EconomicEngineDebugGui::realtimeDataSlot() const
{
	auto key = turnManager->getTurnNumber();
	auto stockExchange = StockExchange::getInstance();

	auto totalData = 0;
	double valueHigh = 0;
	double valueLow = 999999;
	auto haveData = false;
	for (auto checkBox : this->arrayCheckBox)
	{
		auto const graphIndex = checkBox->getGraphIndex();
		// add data to lines:

		ui.customPlot->graph(graphIndex)->addData(
			key, stockExchange->getStockExchangePrice(checkBox->getItemId()));
		// rescale value (vertical) axis to fit the current data:
		totalData += ui.customPlot->graph(graphIndex)->data()->size();


		if (checkBox->isChecked())
		{
			const auto data = ui.customPlot->graph(graphIndex)->data().get();
			auto start = key - this->zoomXAxis;
			if (start < 0)
			{
				start = 0;
			}
			for (auto i = start; i <= key; i++)
			{
				const auto value = data->at(i)->value;
				if (value > 0)
				{
					haveData = true;
					if (value < valueLow)
					{
						valueLow = value;
					}
					if (value > valueHigh)
					{
						valueHigh = value;
					}
				}
			}
		}
	}

	ui.customPlot->rescaleAxes(true);
	if (haveData)
	{
		ui.customPlot->yAxis->setRange(valueLow - valueLow * 0.05, valueHigh + valueHigh * 0.05);
	}

	ui.customPlot->xAxis->setRange(key, this->zoomXAxis, Qt::AlignRight);
	ui.customPlot->replot();


	ui.statusBar->showMessage(
		QString("Total Data points: %1").arg(totalData), 0);
}

void EconomicEngineDebugGui::closeEvent(QCloseEvent* event)
{
	this->turnManager->stop();
	this->economicEngineThread.join();
}
