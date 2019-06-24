#include "sms_bingo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	sms_bingo w;
	w.show();
	return a.exec();
}
