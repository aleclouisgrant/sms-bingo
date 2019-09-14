#include "BingoSpace.h"
#include <QResizeEvent>
#include <QWidget>


BingoSpace::BingoSpace()
{
}


BingoSpace::~BingoSpace()
{
}


void BingoSpace::resizeEvent(QResizeEvent *event)
{
	event->accept();

	if (event->size().width() > event->size().height()) {
		QWidget::resize(event->size().height(), event->size().height());
	}
	else {
		QWidget::resize(event->size().width(), event->size().width());
	}
}