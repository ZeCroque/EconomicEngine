#include "TurnManager.h"
#include <thread>

TurnManager* TurnManager::_instance = nullptr;

TurnManager& TurnManager::getInstance()
{
	if(TurnManager::_instance==nullptr)
	{
		TurnManager::_instance = new TurnManager();
	}
	return *TurnManager::_instance;
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

