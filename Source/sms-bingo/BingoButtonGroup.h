#pragma once

#include <QWidget>
#include <QButtonGroup>
#include <BingoSpace.h>

class BingoButtonGroup : public QButtonGroup
{
	Q_OBJECT

public:
	BingoButtonGroup(QWidget *parent);
	~BingoButtonGroup();

	void setText(int id, QString text);
	void setSpace(int id, BingoSpace *bingoSpace);
	bool checkSpace(int id);
	void selectSpace(int id);
	void setSelectable(int id);

private:
	QString selectedSpaceSS;
	QString selectableSpaceSS;

};
