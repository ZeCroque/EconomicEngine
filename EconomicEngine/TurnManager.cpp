#include "TurnManager.h"
#include <thread>


#include "../EconomicEngineDebugGUI/Wheat.h"
#include "../EconomicEngineDebugGUI/Meat.h"
#include "../EconomicEngineDebugGUI/Wood.h"
#include "../EconomicEngineDebugGUI/Coal.h"
#include "../EconomicEngineDebugGUI/Bread.h"
#include "../EconomicEngineDebugGUI/Steak.h"
#include "../EconomicEngineDebugGUI/Iron.h"
#include "../EconomicEngineDebugGUI/Leather.h"

#include "../EconomicEngineDebugGUI/Farmer.h"
#include "../EconomicEngineDebugGUI/Hunter.h"
#include "../EconomicEngineDebugGUI/Miner.h"
#include "../EconomicEngineDebugGUI/Lumberjack.h"
#include "../EconomicEngineDebugGUI/Butcher.h"
#include "../EconomicEngineDebugGUI/Baker.h"
#include "../EconomicEngineDebugGUI/Bow.h"
#include "../EconomicEngineDebugGUI/ToolMaker.h"

#include "../EconomicEngineDebugGUI/Axe.h"
#include "../EconomicEngineDebugGUI/Hoe.h"
#include "../EconomicEngineDebugGUI/Pickaxe.h"
#include "../EconomicEngineDebugGUI/Sword.h"
#include "../EconomicEngineDebugGUI/WoodPickaxe.h"


#include "Uncountable.h"


TurnManager::TurnManager() : bRunning(false), isStarted(false), turnSecond(1), turnNumber(0),step(1),traderManager(TraderManager::getInstance()),tradableManager(TradableManager::getInstance()),stockExchange(StockExchange::getInstance()){}


void TurnManager::init() const
{
	//Init jobs
	traderManager->registerJob(new Farmer());
	traderManager->registerJob(new Hunter());
	traderManager->registerJob(new Miner());
	traderManager->registerJob(new Baker());
	traderManager->registerJob(new Butcher());
	traderManager->registerJob(new ToolMaker());
	traderManager->registerJob(new Lumberjack());

	//Init tradables
	tradableManager->registerTradable(new Wheat());
	tradableManager->registerTradable(new Meat());
	tradableManager->registerTradable(new Wood());
	tradableManager->registerTradable(new Coal());
	tradableManager->registerTradable(new Bread());
	tradableManager->registerTradable(new Steak());
	tradableManager->registerTradable(new Iron());
	tradableManager->registerTradable(new Leather());

	//Init tools
	tradableManager->registerTradable(new WoodPickaxe());
	tradableManager->registerTradable(new Hoe());
	tradableManager->registerTradable(new Bow());
	tradableManager->registerTradable(new Axe());
	tradableManager->registerTradable(new Pickaxe());
	tradableManager->registerTradable(new Sword());
	
	
	//Init StockExchange
	stockExchange->setKeys(tradableManager->getKeys());

	//Create traders
	traderManager->addTrader(10);
}

void TurnManager::reset() const
{
	traderManager->reset();
	stockExchange->reset();
}

int TurnManager::exec()
{
	this->bRunning = true;
	auto pauseTime = 250;

	while (bRunning)
	{
		if (this->isStarted)
		{
			for (auto i = 0; i < this->step; i++)
			{
				++turnNumber;
				traderManager->doTradersCrafting();
				traderManager->doTradersAsking();
				stockExchange->resolveOffers();
				traderManager->refreshTraders();
			}

			this->notifyObservers();
			pauseTime = 1000 / turnSecond;
		}
		else
		{
			pauseTime = 250;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(pauseTime));
	}
	return 0;
}

void TurnManager::stop()
{
	this->bRunning = false;
}

void TurnManager::setIsStarted(const bool isStarted)
{
	this->isStarted = isStarted;
}

void TurnManager::setTurnSecond(const int turnSecond)
{
	this->turnSecond = turnSecond;
}

int TurnManager::getTurnNumber() const
{
	return this->turnNumber;
}

void TurnManager::setTurnNumber(const int turnNumber)
{
	this->turnNumber = turnNumber;
}

void TurnManager::setStep(const int step)
{
	this->step = step;
}
