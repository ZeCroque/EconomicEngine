#include "EconomicEngineDebugGUI.h"
#include <QtWidgets/QApplication>

int start(int argc, char** argv)
{
	QApplication a(argc, argv);
	EconomicEngineDebugGui w;
	w.show();

	return QApplication::exec();
}

#if defined(WIN32) && defined(NDEBUG)
int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	start(0, nullptr);
	return 0;
}
#else
int main(int argc, char** argv)
{
	return start(argc, argv);
}
#endif


