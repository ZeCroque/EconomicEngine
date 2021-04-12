#include "GameManager.h"


int main()
{
	auto* gameManager = GameManager::getInstance();
	gameManager->Exec();
}