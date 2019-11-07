#include <QFile>
#include "BingoButtonGroup.h"
#include "BingoButton.h"

BingoButtonGroup::BingoButtonGroup(QWidget *parent)
	: QButtonGroup(parent)
{
	QFile selectLocalFile(":/QSS/selectLocalSpace.qss");
	selectLocalFile.open(QFile::ReadOnly);
	selectedLocalSpaceSS = QLatin1String(selectLocalFile.readAll());
	selectLocalFile.close();
	
	QFile selectableFile(":/QSS/selectableSpace.qss");
	selectableFile.open(QFile::ReadOnly);
	selectableSpaceSS = QLatin1String(selectableFile.readAll());
	selectableFile.close();

	QFile selectRemoteFile(":/QSS/selectRemoteSpace.qss");
	selectRemoteFile.open(QFile::ReadOnly);
	selectedRemoteSpaceSS = QLatin1String(selectRemoteFile.readAll());
	selectRemoteFile.close();
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

bool BingoButtonGroup::selectLocalSpace(int id)
{
	BingoButton *btn = (BingoButton *)button(id);

	//if (!(btn->isSelected()) && btn->isSelectable()) { //for testing
	if (!(btn->isSelected())) {
		btn->setStyleSheet(selectedLocalSpaceSS);
		btn->SelectSpace();
		return true;
	}
	return false;
}

bool BingoButtonGroup::selectRemoteSpace(int id)
{
	BingoButton *btn = (BingoButton *)button(id);

	if (!(btn->isSelected())) {
		btn->setStyleSheet(selectedRemoteSpaceSS);
		btn->SelectSpace();
		return true;
	}
	return false;
}