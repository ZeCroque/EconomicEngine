#include "TurnManager.h"
#include "Countable.h"
#include <thread>

#include "../EconomicEngineDebugGUI/Wheat.h"
#include "../EconomicEngineDebugGUI/Meat.h"
#include "../EconomicEngineDebugGUI/Coal.h"
#include "../EconomicEngineDebugGUI/Bread.h"
#include "../EconomicEngineDebugGUI/Steak.h"

#include "../EconomicEngineDebugGUI/Farmer.h"
#include "../EconomicEngineDebugGUI/Hunter.h"
#include "../EconomicEngineDebugGUI/Butcher.h"
#include "../EconomicEngineDebugGUI/Baker.h"


#include "../EconomicEngineDebugGUI/Miner.h"
#include "Uncountable.h"


TurnManager::TurnManager() : bRunning(false), isStarted(false), turnSecond(1), turnNumber(0),step(1),
                             traderManager(TraderManager::getInstance()),
                             tradableManager(TradableManager::getInstance()),
                             stockExchange(StockExchange::getInstance())
{
}


void TurnManager::init() const
{
	//Init jobs
	traderManager->registerJob(new Farmer());
	traderManager->registerJob(new Hunter());
	traderManager->registerJob(new Miner());
	traderManager->registerJob(new Baker());
	traderManager->registerJob(new Butcher());

	//Init tradables
	tradableManager->registerTradable(new Wheat());
	tradableManager->registerTradable(new Meat());
	tradableManager->registerTradable(new Coal());
	tradableManager->registerTradable(new Bread());
	tradableManager->registerTradable(new Steak());

	//Init tools
	tradableManager->registerTradable(new Hoe());

	//Init StockExchange
	stockExchange->setKeys(tradableManager->getKeys());

	//Create traders
	traderManager->addTrader(10);
}

void TurnManager::reset()
{
	//TODO
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
				auto a = stockExchange->betterAsks[typeid(Gold).hash_code()];
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
