#include "GameManager.h"


int main()
{
	auto* gameManager = GameManager::getInstance();
    gameManager->init("./Content/");
	gameManager->exec();
}