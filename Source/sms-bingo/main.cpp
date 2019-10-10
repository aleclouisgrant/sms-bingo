#include "TitleWindow.h"
#include "BoardWindow.h"
#include "HomeWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//TitleWindow window;
	//BoardWindow window;
	HomeWindow *window1 = new HomeWindow();
	HomeWindow *window2 = new HomeWindow();

	//window.showFullScreen();
	window1->show();
	window2->show();

	return a.exec();
}
