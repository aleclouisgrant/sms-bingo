#include <QFile>
#include "BingoButtonGroup.h"
#include "BingoButton.h"

BingoButtonGroup::BingoButtonGroup(QWidget *parent)
	: QButtonGroup(parent)
{
	QFile file(":/QSS/selectspace.qss");
	file.open(QFile::ReadOnly);
	selectSpaceSS = QLatin1String(file.readAll());
	file.close();
}

BingoButtonGroup::~BingoButtonGroup()
{
}

void BingoButtonGroup::setText(int id, QString text)
{
	BingoButton *btn = (BingoButton *)button(id);
	btn->setText(text);
}

void BingoButtonGroup::selectSpace(int id)
{
	BingoButton *btn = (BingoButton *)button(id);
	btn->setEnabled(false);
	btn->setStyleSheet(selectSpaceSS);
}