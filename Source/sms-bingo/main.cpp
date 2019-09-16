#include "TitleWindow.h"
#include "BoardWindow.h"
#include "HomeWindow.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//TitleWindow window;
	BoardWindow window;
	//HomeWindow window;

	//window.showFullScreen();
	window.show();

	return a.exec();
}
