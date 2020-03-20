#include "EconomicEngineDebugGUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	EconomicEngineDebugGUI w;
	w.show();
	return a.exec();
}
