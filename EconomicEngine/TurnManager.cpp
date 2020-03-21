#include "TurnManager.h"

TurnManager* TurnManager::_instance = nullptr;

TurnManager& TurnManager::GetInstance()
{
	if(TurnManager::_instance==nullptr)
	{
		TurnManager::_instance = new TurnManager();
	}
	return *TurnManager::_instance;
}

