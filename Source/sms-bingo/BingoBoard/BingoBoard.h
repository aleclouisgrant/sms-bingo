/*
#pragma once
#include <QWidget>
#include <qgridlayout.h>
#include <BingoButtonGroup.h>
#include <Player.h>

class BingoBoard2 : public QWidget
{
	Q_OBJECT

public:
	BingoBoard2(int boardSize, Player *player, QWidget *parent);
	~BingoBoard2();

	Player* getPlayer();

	void PopulateBoard();
	void CheckBoard();
	void SelectSpace(int id);

signals:
	void SpaceSelected(int id);

private:
	int m_boardSize; 
	Player *m_player;

	QWidget *m_centralWidget;
	BingoButtonGroup *m_bingoButtonGroup;
	QGridLayout *m_bingoGrid;
};
*/