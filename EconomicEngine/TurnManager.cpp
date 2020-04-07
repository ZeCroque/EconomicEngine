#include "TurnManager.h"
#include <thread>
#include <thread>

//#include "StockExchange.h"

<<<<<<< HEAD
TurnManager::TurnManager() : bRunning(false), traderManager(TraderManager::getInstance()), tradableManager(TradableManager::getInstance()) {}
=======
TurnManager::TurnManager() : bRunning(false), turnSecond(1), turnNumber(0), traderManager(TraderManager::getInstance()),
                             tradableManager(TradableManager::getInstance())
{
}
>>>>>>> origin

void TurnManager::init() const
{
	traderManager->addTrader(10);
	traderManager->registerJob(new Farmer());

	tradableManager->registerTradable(new Bread());
	tradableManager->registerTradable(new Wheat());

	//StockExchange stockExchange(tradableManager->getKeys());
<<<<<<< HEAD
	
=======
>>>>>>> origin
}

void TurnManager::reset()
{
	//TODO
}


//TODO turncount defined by economic engine
int TurnManager::exec()
{
	this->bRunning = true;
	while (bRunning)
	{
		++turnNumber;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / turnSecond));
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
