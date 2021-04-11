#include <iostream>

#include "EconomicEngineDebugGUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	EconomicEngineDebugGui w;
	w.show();

	return QApplication::exec();
}
