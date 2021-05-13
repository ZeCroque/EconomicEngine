#include "GameManager.h"

#if defined(WIN32) && defined(NDEBUG)
#include <Windows.h>
#endif

void start()
{
	auto* gameManager = GameManager::getInstance();
    gameManager->init("./Content/");
	gameManager->exec();
}

#if defined(WIN32) && defined(NDEBUG)
int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	start();
	return 0;
}
#else
int main()
{
	start();
	return 0;
}
#endif