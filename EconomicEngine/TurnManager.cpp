#include "TurnManager.h"
#include <thread>

#include "Trader.h"

void TurnManager::init()
{

	
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

