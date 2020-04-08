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
			checkBox->setItemId(typeid(itemsKeys[i]).hash_code());
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
			connect(checkBox, SIGNAL(clicked()), this,
			        SLOT(setGraphVisibility()));

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
	const auto key = turnManager->getTurnNumber();
	auto stockExchange = StockExchange::getInstance();
	
	auto totalData = 0;
	for (auto checkBox : this->arrayCheckBox)
	{
		auto const graphIndex = checkBox->getGraphIndex();
		// add data to lines:
		
		ui.customPlot->graph(graphIndex)->addData(
			key, stockExchange->getStockExchangePrice(checkBox->getItemId()));

		// rescale value (vertical) axis to fit the current data:
		ui.customPlot->graph(graphIndex)->rescaleValueAxis(false);
		totalData += ui.customPlot->graph(graphIndex)->data()->size();
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
