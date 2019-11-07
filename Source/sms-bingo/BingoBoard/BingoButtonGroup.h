#pragma once

#include <QWidget>
#include <QButtonGroup>
#include "BingoBoard/BingoSpace.h"

class BingoButtonGroup : public QButtonGroup
{
	Q_OBJECT

public:
	BingoButtonGroup(QWidget *parent);
	~BingoButtonGroup();

	void setText(int id, QString text);
	void setSpace(int id, BingoSpace *bingoSpace);
	bool checkSpace(int id);
	bool selectLocalSpace(int id);
	bool selectRemoteSpace(int id);
	void setSelectable(int id);

private:
	QString selectedLocalSpaceSS;
	QString selectedRemoteSpaceSS;
	QString selectableSpaceSS;

};
