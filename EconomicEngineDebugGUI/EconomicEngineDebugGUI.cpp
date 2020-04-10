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

	doInit();

	this->zoomXAxis = ui.horSlidZoomXAxis->value();
	connect(ui.horSlidZoomXAxis,SIGNAL(valueChanged(int)), this,SLOT(setZoomXAxis(int)));

	turnManager->setTurnSecond(ui.horSlidSpeed->value());
	connect(ui.horSlidSpeed, SIGNAL(valueChanged(int)), this, SLOT(setSpeed(int)));

	turnManager->setStep(ui.horSlidStep->value());
	connect(ui.horSlidStep, SIGNAL(valueChanged(int)), this, SLOT(setStep(int)));

	ui.horSlidXNav->setMaximum(ui.horSlidZoomXAxis->value());
	ui.horSlidXNav->setValue(ui.horSlidZoomXAxis->value());
	connect(ui.horSlidXNav, SIGNAL(valueChanged(int)), this, SLOT(useXSlider(int)));

	connect(ui.pBStart, SIGNAL(clicked()), this,SLOT(toggleStart()));
	connect(ui.pBReset, SIGNAL(clicked()), this, SLOT(doReset()));

	connect(ui.radBRealTime, SIGNAL(clicked()), this, SLOT(setMode()));
	connect(ui.radStepByStep, SIGNAL(clicked()), this, SLOT(setMode()));

	connect(this, SIGNAL(nextTurn()), this, SLOT(updateUiSlot()));
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

void EconomicEngineDebugGui::setGraphVisibility()
{
	const auto checkBox = qobject_cast<GraphManager*>(QObject::sender());
	ui.customPlot->graph(checkBox->getGraphIndex())->setVisible(checkBox->isChecked());
	setYRange();
}

void EconomicEngineDebugGui::setZoomXAxis(const int value)
{
	this->zoomXAxis = value;
	setXRange();
	setYRange();
}

void EconomicEngineDebugGui::setYRange()
{
	auto haveData = false;
	const auto key = ui.horSlidXNav->value();
	double valueHigh = 0;
	double valueLow = 999999;
	for (auto checkBox : this->arrayCheckBox)
	{
		auto const graphIndex = checkBox->getGraphIndex();

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

	if (haveData)
	{
		ui.customPlot->yAxis->setRange(valueLow - valueLow * 0.05, valueHigh + valueHigh * 0.05);
	}
	ui.customPlot->replot();
}

void EconomicEngineDebugGui::setXRange() const
{
	const auto key = turnManager->getTurnNumber();
	ui.horSlidXNav->setMaximum(key);
	if (key > this->zoomXAxis)
	{
		ui.horSlidXNav->setMinimum(this->zoomXAxis);
	}

	if (ui.horSlidXNav->value() >= key - ui.horSlidStep->value())
	{
		ui.horSlidXNav->setValue(key);
		ui.customPlot->xAxis->setRange(key, this->zoomXAxis, Qt::AlignRight);
	}
	else
	{
		auto const center = (ui.customPlot->xAxis->range().upper + ui.customPlot->xAxis->range().lower) / 2;
		ui.customPlot->xAxis->setRange(center, this->zoomXAxis, Qt::AlignCenter);
	}

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

void EconomicEngineDebugGui::useXSlider(int value)
{
	ui.customPlot->xAxis->setRange(ui.horSlidXNav->value(), this->zoomXAxis, Qt::AlignRight);
	setYRange();
	ui.customPlot->replot();
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

void EconomicEngineDebugGui::setMode() const
{
	if (!ui.radBRealTime->isChecked())
	{
		ui.pBStart->setChecked(false);
	}
	toggleStart();
}

void EconomicEngineDebugGui::doReset()
{
	for (auto graphManager : arrayCheckBox)
	{
		ui.customPlot->removeGraph(graphManager->getGraphIndex());
	}
	arrayCheckBox.clear();

	ui.customPlot->clearGraphs();
	ui.customPlot->xAxis->setRange(0, 5);
	ui.customPlot->replot();

	ui.pBStart->setChecked(false);
	setMode();

	turnManager->reset();
	doInit();
}

void EconomicEngineDebugGui::doInit()
{
	const auto tradableManager = TradableManager::getInstance();
	auto itemsName = tradableManager->getTradablesName();
	auto itemsKeys = tradableManager->getKeys();

	auto row = 0;
	auto column = 0;
	for (auto i = 0; i < itemsName.size(); ++i)
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

void EconomicEngineDebugGui::updateUiSlot()
{
	const auto key = turnManager->getTurnNumber();
	auto stockExchange = StockExchange::getInstance();

	auto totalData = 0;

	for (auto checkBox : this->arrayCheckBox)
	{
		auto const graphIndex = checkBox->getGraphIndex();

		ui.customPlot->graph(graphIndex)->addData(
			key, stockExchange->getStockExchangePrice(checkBox->getItemId()));

		totalData += ui.customPlot->graph(graphIndex)->data()->size();
	}

	setYRange();
	setXRange();

	ui.statusBar->showMessage(
		QString("Total Data points: %1").arg(totalData), 0);

	setMode();
}

void EconomicEngineDebugGui::closeEvent(QCloseEvent* event)
{
	this->turnManager->stop();
	this->economicEngineThread.join();
}
