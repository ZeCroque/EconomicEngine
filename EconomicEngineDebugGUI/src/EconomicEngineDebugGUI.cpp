#include "EconomicEngineDebugGUI.h"
#include "qcustomplot.h"
#include <thread>
#include "GraphManager.h"

EconomicEngineDebugGui::EconomicEngineDebugGui(QWidget* parent)
	: QMainWindow(parent)
{	
	turnManager = EconomicEngine::getInstance();
	turnManager->getAsksResolvedSignal().connect(this, &EconomicEngineDebugGui::nextTurn);

	traderManager = TraderManager::getInstance();

#ifdef STANDALONE_MODE
	economicEngineThread = std::thread([](EconomicEngine* newTurnManager)-> int
	{
        newTurnManager->init("./Content/Prefabs/");
		return newTurnManager->exec(100);
	}, turnManager);
#endif
	
	ui.setupUi(this);

	doInit();

	zoomXAxis = ui.horSlidZoomXAxis->value();
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
	turnManager = nullptr;
}

void EconomicEngineDebugGui::setGraphVisibility()
{
	const auto checkBox = qobject_cast<GraphManager*>(QObject::sender());
	ui.customPlot->graph(checkBox->getGraphIndex())->setVisible(checkBox->isChecked());
	setYRange();
}

void EconomicEngineDebugGui::setZoomXAxis(const int value)
{
	zoomXAxis = value;
	setXRange();
	setYRange();
}

void EconomicEngineDebugGui::setYRange()
{
	auto haveData = false;
	double valueHigh = 0;
	double valueLow = 999999;
	for (auto checkBox : this->arrayCheckBox)
	{
		auto const graphIndex = checkBox->getGraphIndex();

		if (checkBox->isChecked())
		{
			const auto data = ui.customPlot->graph(graphIndex)->data().get();
			auto start = ui.customPlot->xAxis->range().lower;
			const auto end = ui.customPlot->xAxis->range().upper;
			if (start < 0)
			{
				start = 0;
			}
			for (int i = start; i <= end; i++)
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
	if (key > zoomXAxis)
	{
		ui.horSlidXNav->setMinimum(zoomXAxis);
	}

	if (ui.horSlidXNav->value() >= key - ui.horSlidStep->value())
	{
		ui.horSlidXNav->setValue(key);
		ui.customPlot->xAxis->setRange(key, zoomXAxis, Qt::AlignRight);
	}
	else
	{
		auto const center = (ui.customPlot->xAxis->range().upper + ui.customPlot->xAxis->range().lower) / 2;
		ui.customPlot->xAxis->setRange(center, zoomXAxis, Qt::AlignCenter);
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

void EconomicEngineDebugGui::useXSlider(int)
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
	const auto job = arrayJobs.at(ui.cBKill->currentIndex());
	traderManager->kill(job->getJobId(), ui.sBKill->value());
	updateUiJobs();
}

void EconomicEngineDebugGui::doAdd()
{
	const auto job = arrayJobs.at(ui.cBKill->currentIndex());
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


	arrayJobs.clear();
	while (ui.gridLayJobs->count() > 0)
	{
		QLayoutItem* item = ui.gridLayJobs->takeAt(0);
		QWidget* widget = item->widget();
		delete widget;
	}
	turnManager->reset(ui.sBTraderNumber->value());
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
	ui.gridLayJobs->addWidget(new QLabel("Numbers |"), 0, 1);
	ui.gridLayJobs->addWidget(new QLabel("Avg. money |"), 0, 2);
	ui.gridLayJobs->addWidget(new QLabel("Avg. food |"), 0, 3);
	ui.gridLayJobs->addWidget(new QLabel("Birth |"), 0, 4);
	ui.gridLayJobs->addWidget(new QLabel("Dead"), 0, 5);
	for (const auto& job : traderManager->getJobList())
	{
		auto jobManager = new JobManager(job.first, QString::fromStdString(job.second));

		jobManager->lbName = new QLabel(jobManager->getJobName());

		auto number = QString::number(traderManager->getJobCount(jobManager->getJobId()));
		jobManager->lbNumber = new QLabel(number);

		jobManager->lbMoneyAverage = new QLabel(QString::number(0));
		jobManager->lbFoodAverage = new QLabel(QString::number(0));
		jobManager->lbBirth = new QLabel(QString::number(0));
		jobManager->lbDead = new QLabel(QString::number(0));

		const auto layRow = static_cast<int>(arrayJobs.size()) + 1;
		ui.gridLayJobs->addWidget(jobManager->lbName, layRow, 0);
		ui.gridLayJobs->addWidget(jobManager->lbNumber, layRow, 1);
		ui.gridLayJobs->addWidget(jobManager->lbMoneyAverage, layRow, 2);
		ui.gridLayJobs->addWidget(jobManager->lbFoodAverage, layRow, 3);
		ui.gridLayJobs->addWidget(jobManager->lbBirth, layRow, 4);
		ui.gridLayJobs->addWidget(jobManager->lbDead, layRow, 5);

		ui.cBKill->addItem(jobManager->getJobName());
		arrayJobs.push_back(jobManager);
	}
	updateUiJobs();
}

void EconomicEngineDebugGui::updateUiJobs()
{
	const auto cbJob = arrayJobs.at(ui.cBKill->currentIndex());
	const auto traderCount = traderManager->getJobCount(cbJob->getJobId());
	ui.sBKill->setMaximum(traderCount);

	for (auto job : arrayJobs)
	{
		const auto jobId = job->getJobId();

		auto number = QString::number(traderManager->getJobCount(jobId));
		auto money = QString::number(traderManager->getMoneyMeanByJob(jobId));
		auto food = QString::number(traderManager->getFoodLevelMeanByJob(jobId));

		const auto demography = traderManager->getDemographyByJob(jobId);
		auto birth = QString::number(demography.first);
		auto dead = QString::number(demography.second);

		job->lbNumber->setText(number);
		job->lbMoneyAverage->setText(money);
		job->lbFoodAverage->setText(food);
		job->lbBirth->setText(birth);
		job->lbDead->setText(dead);
	}
}

void EconomicEngineDebugGui::updateUiSlot()
{
	const auto key = turnManager->getTurnCount();
	const auto stockExchange = StockExchange::getInstance();

	auto totalData = 0;

	auto const step = ui.horSlidStep->value();

	for (auto checkBox : this->arrayCheckBox)
	{
		auto const graphIndex = checkBox->getGraphIndex();

		auto i = key - step;
		for (const auto& data : stockExchange->getStockExchangePrice(checkBox->getItemId(), step))
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
#ifdef STANDALONE_MODE
	this->turnManager->stop();
	this->economicEngineThread.join();
#endif
}
