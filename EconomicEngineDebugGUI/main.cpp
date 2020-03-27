#include "EconomicEngineDebugGUI.h"
#include <QtWidgets/QApplication>
#include "TurnManager.h"

int main(int argc, char *argv[])
{
	TurnManager* turnManager = &TurnManager::getInstance();
	
	QApplication a(argc, argv);
	EconomicEngineDebugGui w;
	w.show();
	
	return QApplication::exec();
}
