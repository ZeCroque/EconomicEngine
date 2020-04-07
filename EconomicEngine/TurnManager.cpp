#include "TurnManager.h"
#include <thread>
#include <thread>

//#include "StockExchange.h"

TurnManager::TurnManager() : bRunning(false), traderManager(TraderManager::getInstance()), tradableManager(TradableManager::getInstance()) {}

void TurnManager::init() const
{
	traderManager->addTrader(10);
	traderManager->registerJob(new Farmer());

	tradableManager->registerTradable(new Bread());
	tradableManager->registerTradable(new Wheat());

	//StockExchange stockExchange(tradableManager->getKeys());
	
}

void TurnManager::reset()
{
	//TODO
}


//TODO turncount defined by economic engine
int TurnManager::exec()
{
	this->bRunning = true;
	while(bRunning)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		this->notifyObservers();
		traderManager->doTradersCrafting();
		traderManager->refreshTraders();
	}
	return 0;
}

void TurnManager::stop()
{
	this->bRunning = false;
}

