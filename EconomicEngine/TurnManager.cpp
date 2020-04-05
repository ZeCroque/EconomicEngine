#include "TurnManager.h"

#include <thread>

TurnManager::TurnManager() : bRunning(false), traderManager(TraderManager::getInstance()) {}

void TurnManager::init()
{
	traderManager->addTrader(10);
	traderManager->registerJob(new Job());
	traderManager->refreshTraders();

	int i = 42;
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
		std::this_thread::sleep_for(std::chrono::seconds(1));
		this->notifyObservers();		
	}
	return 0;
}

void TurnManager::stop()
{
	this->bRunning = false;
}

