#include "EconomicEngineDebugGUI.h"
#include "qcustomplot.h"

#include "EconomicEngine.h"
#include "Traders/Trader.h"
#include "GraphManager.h"
#include "JobManager.h"

#ifndef STANDALONE_MODE
	#include "GameManager.h"
#else
#include <thread>

constexpr int TPS = 60;
#endif

EconomicEngineDebugGui::EconomicEngineDebugGui(QWidget *parent)
        : QMainWindow(parent), asksResolutionCount(0)
#ifdef STANDALONE_MODE
		, isRunning(false), hasBeenReset(false), hasEverRun(false), speedFactor(1.f)
#endif
{
    ui.setupUi(this);

#ifdef STANDALONE_MODE
	ui.label_2->setVisible(false);
#else
    ui.pBStart->setChecked(true);
    ui.pBStart->setText("Stop");
#endif

    zoomXAxis = ui.horSlidZoomXAxis->value();
    connect(ui.horSlidZoomXAxis, SIGNAL(valueChanged(int)), this, SLOT(setZoomXAxis(int)));


    connect(ui.horSlidSpeed, SIGNAL(valueChanged(int)), this, SLOT(setSpeed(int)));
	
    ui.horSlidXNav->setMaximum(ui.horSlidZoomXAxis->value());
    ui.horSlidXNav->setValue(ui.horSlidZoomXAxis->value());
    connect(ui.horSlidXNav, SIGNAL(valueChanged(int)), this, SLOT(useXSlider(int)));

    connect(ui.pBStart, SIGNAL(clicked()), this, SLOT(toggleStart()));
    connect(ui.pBReset, SIGNAL(clicked()), this, SLOT(doReset()));

    connect(ui.pBAdd, SIGNAL(clicked()), this, SLOT(doAdd()));
    connect(ui.pBKill, SIGNAL(clicked()), this, SLOT(doKill()));

    connect(this, SIGNAL(nextTurn()), this, SLOT(updateUiSlot()));

	auto* economicEngine = EconomicEngine::getInstance();
    economicEngine->getStockExchange().getAskResolvedSignal().connect(this, &EconomicEngineDebugGui::nextTurn);

#ifdef STANDALONE_MODE
    economicEngine->init("./Content/Prefabs/");
    economicEngineThread = std::thread([this]()
    {
	    // ReSharper disable once CppPossiblyErroneousEmptyStatements
	    while(!hasEverRun); //NOLINT(clang-diagnostic-empty-body)
    	
    	const float deltaTime = 1.f / TPS;
    	while(isRunning)
    	{		
    		EconomicEngine::getInstance()->update(deltaTime * speedFactor);
    		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(deltaTime * 1000.f)));
    	}
    });

	economicEngine->getTraderManager().getTraderAddedSignal().connect([this](Trader* trader)
	{
		trader->getMoveToRequestSignal().connect([this, trader](Position position)
		{
			auto movementSimulationThread = std::thread([this, trader, position]()
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>((EconomicEngine::getInstance()->getBaseActionTime() * 1000.f / speedFactor))));
				if(!hasBeenReset)
				{
					trader->setPosition(position);
				}
			});
			movementSimulationThread.detach();
		});
	});
	economicEngine->start(100);
#endif

	doInit();
}

void EconomicEngineDebugGui::showEvent(QShowEvent *event)
{
#ifndef STANDALONE_MODE
    initializedSignal();
#endif
}


void EconomicEngineDebugGui::closeEvent(QCloseEvent *event)
{
	EconomicEngine::getInstance()->getStockExchange().getAskResolvedSignal().disconnectAll();
#ifdef STANDALONE_MODE
	hasEverRun = true;
	isRunning = false;
	economicEngineThread.join();
#endif
}

void EconomicEngineDebugGui::setGraphVisibility()
{
    const auto checkBox = qobject_cast<GraphManager *>(QObject::sender());
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
	for (auto* checkBox : arrayCheckBox)
	{
		auto const graphIndex = checkBox->getGraphIndex();

		if (checkBox->isChecked())
		{	
			const auto* data = ui.customPlot->graph(graphIndex)->data().get();
			auto start = ui.customPlot->xAxis->range().lower;
			const auto end = ui.customPlot->xAxis->range().upper;
			if (start < 0)
			{
				start = 0;
			}
			for (int i = static_cast<int>(start); i <= end; i++)
			{
				if (const auto value = data->at(i)->value; value > 0)
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
    const auto key = asksResolutionCount;
    ui.horSlidXNav->setMaximum(key);
    if (key > zoomXAxis)
    {
        ui.horSlidXNav->setMinimum(zoomXAxis);
    }

    if (ui.horSlidXNav->value() >= key - 1)
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

// ReSharper disable once CppMemberFunctionMayBeStatic
void EconomicEngineDebugGui::setSpeed(const int value)
{
#ifdef STANDALONE_MODE
	speedFactor = static_cast<float>(value);
#else
	GameManager::getInstance()->setSpeedFactor(static_cast<float>(value));
#endif
}

void EconomicEngineDebugGui::useXSlider(int)
{
	ui.customPlot->xAxis->setRange(ui.horSlidXNav->value(), zoomXAxis, Qt::AlignRight);
	setYRange();
	ui.customPlot->replot();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void EconomicEngineDebugGui::toggleStart()
{
    if (ui.pBStart->isChecked())
    {
        ui.pBStart->setText("Stop");
#ifndef STANDALONE_MODE
    	GameManager::getInstance()->resume();
#else
    	hasEverRun = true;
    	isRunning = true;
    	hasBeenReset = false;
    	EconomicEngine::getInstance()->resume();
#endif
    }
    else
    {
        ui.pBStart->setText("Start");
#ifndef STANDALONE_MODE
    	GameManager::getInstance()->pause();
#else
    	EconomicEngine::getInstance()->pause();
#endif
    }
}

void EconomicEngineDebugGui::doKill()
{
	const auto job = arrayJobs.at(ui.cBKill->currentIndex());
	EconomicEngine::getInstance()->getTraderManager().markForKill(job->getJobId(), ui.sBKill->value());
	updateUiJobs();
}

void EconomicEngineDebugGui::doAdd()
{
    const auto job = arrayJobs.at(ui.cBKill->currentIndex());
    EconomicEngine::getInstance()->getTraderManager().addTrader(ui.sBAdd->value(), job->getJobId());
    updateUiJobs();
}

void EconomicEngineDebugGui::doReset()
{
	asksResolutionCount = 0;
	ui.pBStart->setChecked(false);
    ui.pBStart->setText("Start");
#ifndef STANDALONE_MODE
	GameManager::getInstance()->pause();
#else
	hasBeenReset = true;
	auto* economicEngine = EconomicEngine::getInstance();
	economicEngine->getStockExchange().getAskResolvedSignal().disconnectAll();
	economicEngine->pause();
#endif

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
    ui.horSlidXNav->setMaximum(5);
    ui.horSlidXNav->setSliderPosition(5);


	arrayJobs.clear();
	while (ui.gridLayJobs->count() > 0)
	{
		QLayoutItem* item = ui.gridLayJobs->takeAt(0);
		QWidget* widget = item->widget();
		delete widget;
	}

#ifndef STANDALONE_MODE
	GameManager::getInstance()->reset(ui.sBTraderNumber->value());
#else
	economicEngine->reset(ui.sBTraderNumber->value());
	economicEngine->getStockExchange().getAskResolvedSignal().connect(this, &EconomicEngineDebugGui::nextTurn);
#endif
	
	doInit();
}

void EconomicEngineDebugGui::doInit()
{
	const auto& tradableManager = EconomicEngine::getInstance()->getTradableFactory();
	auto itemsName = tradableManager.getTradablesName();
	auto itemsKeys = tradableManager.getKeys();

	auto row = 0;
	auto column = 0;
	const auto itemNameSize = static_cast<int>(itemsName.size());
	for (int i = 0; i < itemNameSize; ++i)
	{
		auto* checkBox = new GraphManager(this);
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

		arrayCheckBox.push_back(checkBox);

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

	for (auto & traderManager = EconomicEngine::getInstance()->getTraderManager(); const auto& job : traderManager.getJobList())
	{
		auto jobManager = new JobManager(job.first, QString::fromStdString(job.second));

		jobManager->lbName = new QLabel(jobManager->getJobName());

		auto number = QString::number(traderManager.getJobCount(jobManager->getJobId()));
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
    auto &traderManager = EconomicEngine::getInstance()->getTraderManager();

    const auto traderCount = traderManager.getJobCount(cbJob->getJobId());
    ui.sBKill->setMaximum(traderCount);

    for (auto job : arrayJobs)
    {
        const auto jobId = job->getJobId();

        auto number = QString::number(traderManager.getJobCount(jobId));
        auto money = QString::number(static_cast<double>(traderManager.getMoneyMeanByJob(jobId)));
        auto food = QString::number(static_cast<double>(traderManager.getFoodLevelMeanByJob(jobId)));

        const auto demography = traderManager.getDemographyByJob(jobId);
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
	const auto stockExchange = EconomicEngine::getInstance()->getStockExchange();
	++asksResolutionCount;
	
	auto totalData = 0;

	for (auto checkBox : arrayCheckBox)
	{
		auto const graphIndex = checkBox->getGraphIndex();


		for (auto i = asksResolutionCount - 1; const auto& data : stockExchange.getStockExchangePrice(checkBox->getItemId(), 1))
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
}

#ifndef STANDALONE_MODE
const Signal<> &EconomicEngineDebugGui::getInitializedSignal() const
{
    return initializedSignal;
}
#endif
