#include "TurnManager.h"
#include <thread>


void TurnManager::init()
{
	//TODO
}

void TurnManager::reset()
{
	//TODO
}

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

