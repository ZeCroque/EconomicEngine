#include "EconomicEngineDebugGUI.h"
#include <QtWidgets/QApplication>
#include "TurnManager.h"

int main(int argc, char *argv[])
{
	TurnManager* turnManager = &TurnManager::GetInstance();
	
	QApplication a(argc, argv);
	EconomicEngineDebugGUI w;
	w.show();
	return a.exec();
}
