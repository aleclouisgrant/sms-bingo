#pragma once
#include <QWidget>
#include <qgridlayout.h>
#include <BingoButtonGroup.h>


class BingoBoard : public QWidget
{
	Q_OBJECT

public:
	BingoBoard(int boardSize, QWidget *parent);
	~BingoBoard();

	void PopulateBoard();

private:
	int m_boardSize; 

	QWidget *m_centralWidget;
	BingoButtonGroup *m_bingoButtonGroup;
	QGridLayout *m_bingoGrid;
};

