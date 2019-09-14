#pragma once

#include <QWidget>
#include <QButtonGroup>

class BingoButtonGroup : public QButtonGroup
{
	Q_OBJECT

public:
	BingoButtonGroup(QWidget *parent);
	~BingoButtonGroup();

	void setText(int id, QString text);
	void selectSpace(int id);

private:
	QString selectSpaceSS;

};
