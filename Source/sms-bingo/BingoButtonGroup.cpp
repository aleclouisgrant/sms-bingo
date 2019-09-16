#include <QFile>
#include "BingoButtonGroup.h"
#include "BingoButton.h"

BingoButtonGroup::BingoButtonGroup(QWidget *parent)
	: QButtonGroup(parent)
{
	QFile file(":/QSS/selectspace.qss");
	file.open(QFile::ReadOnly);
	selectedSpaceSS = QLatin1String(file.readAll());
	file.close();
	
	QFile otherfile(":/QSS/selectableSpace.qss");
	otherfile.open(QFile::ReadOnly);
	selectableSpaceSS = QLatin1String(otherfile.readAll());
	otherfile.close();
}

BingoButtonGroup::~BingoButtonGroup()
{
}

void BingoButtonGroup::setText(int id, QString text)
{
	BingoButton *btn = (BingoButton *)button(id);
	btn->setText(text);
}

void BingoButtonGroup::setSpace(int id, BingoSpace *bingoSpace)
{
	BingoButton *btn = (BingoButton *)button(id);
	btn->setBingoSpace(bingoSpace);
}

bool BingoButtonGroup::checkSpace(int id) 
{
	BingoButton *btn = (BingoButton *)button(id);
	return btn->CheckSpace();
}

void BingoButtonGroup::setSelectable(int id) 
{
	BingoButton *btn = (BingoButton *)button(id);
	btn->setStyleSheet(selectableSpaceSS);
}

bool BingoButtonGroup::selectSpace(int id)
{
	BingoButton *btn = (BingoButton *)button(id);

	if (!(btn->isSelected()) && btn->isSelectable()) {
		btn->setStyleSheet(selectedSpaceSS);
		btn->SelectSpace();
		return true;
	}
	return false;

}