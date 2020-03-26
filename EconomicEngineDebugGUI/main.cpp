#include "EconomicEngineDebugGUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	EconomicEngineDebugGui w;
	w.show();

	TurnManager* turnManager = &TurnManager::getInstance();
	turnManager->addObserver(&w);
	w.turnManager = turnManager;
	
	std::thread economicEngineThread([](TurnManager* turnManager)->int {return turnManager->exec();}, turnManager);
	w.economicEngineThread = &economicEngineThread;
	return QApplication::exec();
}
