#include "QWindows/TitleWindow.h"
#include "QWindows/BoardWindow.h"
#include "QWindows/HomeWindow.h"
#include "MainServer.h"
#include "MainClient.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//TitleWindow window;
	//BoardWindow window;
	MainServer ms;
	MainClient mc1;
	MainClient mc2;

	//HomeWindow *window1 = new HomeWindow();
	//HomeWindow *window2 = new HomeWindow();

	//window.showFullScreen();
	//window.show();
	//window1->show();
	//window2->show();

	ms.show();
	mc1.setMinimumSize(700, 950);
	mc1.show();
	mc2.setMinimumSize(700, 950);
	mc2.show();

	return a.exec();
}
