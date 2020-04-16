#include "EconomicEngineDebugGUI.h"
#include "qcustomplot.h"
#include "EconomicEngine.h"
#include <QCloseEvent>
#include <thread>
#include "GraphManager.h"
#include "TraderManager.h"
#include "TradableManager.h"


EconomicEngineDebugGui::EconomicEngineDebugGui(QWidget* parent)
	: QMainWindow(parent)
{
	turnManager = DebugEconomicEngine::getInstance();
	turnManager->addObserver(this);
	economicEngineThread = std::thread([](DebugEconomicEngine* turnManager)-> int
	{
		turnManager->init();
		return turnManager->exec(300);
	}, turnManager);

	traderManager = TraderManager::getInstance();
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

	connect(ui.pBAdd, SIGNAL(clicked()), this, SLOT(doAdd()));
	connect(ui.pBKill, SIGNAL(clicked()), this, SLOT(doKill()));

	connect(ui.cBKill,SIGNAL(valueChanged()), this,SLOT(updateUiJobs()));

	connect(this, SIGNAL(nextTurn()), this, SLOT(updateUiSlot()));
}

EconomicEngineDebugGui::~EconomicEngineDebugGui()
{
	DebugEconomicEngine::destroyInstance();
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
			auto start = ui.customPlot->xAxis->range().lower;
			auto end = ui.customPlot->xAxis->range().upper;
			if (start < 0)
			{
				start = 0;
			}
			for (auto i = start; i <= end; i++)
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
	const auto key = turnManager->getTurnCount();
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
		turnManager->resume();
	}
	else
	{
		ui.pBStart->setText("Start");
		turnManager->pause();
	}
}

void EconomicEngineDebugGui::setMode() const
{
	if (!ui.radBRealTime->isChecked())
	{
		ui.pBStart->setChecked(false);
	}
	toggleStart();
}

void EconomicEngineDebugGui::doKill()
{
	const auto job = this->arrayJobs.at(ui.cBKill->currentIndex());
	traderManager->kill(job->getJobId(), ui.sBKill->value());
	updateUiJobs();
}

void EconomicEngineDebugGui::doAdd()
{
	const auto job = this->arrayJobs.at(ui.cBKill->currentIndex());
	traderManager->addTrader(ui.sBAdd->value(), job->getJobId());
	updateUiJobs();
}

void EconomicEngineDebugGui::doReset()
{
	ui.pBStart->setChecked(false);
	setMode();


	for (auto graphManager : arrayCheckBox)
	{
		ui.customPlot->removeGraph(graphManager->getGraphIndex());
	}
	arrayCheckBox.clear();
	arrayJobs.clear();
	ui.cBKill->clear();

	ui.customPlot->clearGraphs();
	ui.customPlot->xAxis->setRange(0, 5);
	ui.customPlot->replot();

	turnManager->reset(300);

	arrayJobs.clear();
	while (ui.gridLayJobs->count() > 0)
	{
		QLayoutItem* item = ui.gridLayJobs->takeAt(0);
		QWidget* widget = item->widget();
		delete widget;
	}
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


	ui.gridLayJobs->addWidget(new QLabel("Jobs"), 0, 0);
	ui.gridLayJobs->addWidget(new QLabel("Numbers"), 0, 1);
	ui.gridLayJobs->addWidget(new QLabel("Avg. money"), 0, 2);
	for (const auto& job : traderManager->getJobList())
	{
		auto jobManager = new JobManager(job.first, QString::fromStdString(job.second));

		jobManager->lbName = new QLabel(jobManager->getJobName());

		auto number = QString::number(traderManager->getJobCount(jobManager->getJobId()));
		jobManager->lbNumber = new QLabel(number);

		jobManager->lbMoneyAverage = new QLabel(QString::number(0));

		ui.gridLayJobs->addWidget(jobManager->lbName, arrayJobs.size() + 1, 0);
		ui.gridLayJobs->addWidget(jobManager->lbNumber, arrayJobs.size() + 1, 1);

		ui.cBKill->addItem(jobManager->getJobName());
		this->arrayJobs.push_back(jobManager);
	}
	updateUiJobs();
}

void EconomicEngineDebugGui::updateUiJobs()
{
	const auto cbJob = this->arrayJobs.at(ui.cBKill->currentIndex());
	const auto traderCount = traderManager->getJobCount(cbJob->getJobId());
	ui.sBKill->setMaximum(traderCount);

	for (auto job : arrayJobs)
	{
		auto number = QString::number(traderManager->getJobCount(job->getJobId()));
		job->lbNumber->setText(number);
	}
}

void EconomicEngineDebugGui::updateUiSlot()
{
	const auto key = turnManager->getTurnCount();
	auto stockExchange = StockExchange::getInstance();

	auto totalData = 0;

	auto const step = ui.horSlidStep->value();

	for (auto checkBox : this->arrayCheckBox)
	{
		auto const graphIndex = checkBox->getGraphIndex();

		auto i = key - step;
		for (auto data : stockExchange->getStockExchangePrice(checkBox->getItemId(), step))
		{
			ui.customPlot->graph(graphIndex)->addData(i, data.getPrice());
			i++;
		}

		totalData += ui.customPlot->graph(graphIndex)->data()->size();
	}

	setYRange();
	setXRange();
	updateUiJobs();

	ui.statusBar->showMessage(
		QString("Total Data points: %1").arg(totalData), 0);

	setMode();
}

void EconomicEngineDebugGui::closeEvent(QCloseEvent* event)
{
	this->turnManager->stop();
	this->economicEngineThread.join();
}
