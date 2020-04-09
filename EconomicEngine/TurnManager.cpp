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


TurnManager::TurnManager() : bRunning(false), turnSecond(1), turnNumber(0), traderManager(TraderManager::getInstance()),
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
	while (bRunning)
	{
		auto a = stockExchange->betterAsks[typeid(Gold).hash_code()];
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / turnSecond));
		++turnNumber;
		traderManager->doTradersCrafting();
		traderManager->doTradersAsking();

		stockExchange->resolveOffers();
		traderManager->refreshTraders();
		this->notifyObservers();
	}
	return 0;
}

void TurnManager::stop()
{
	this->bRunning = false;
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
