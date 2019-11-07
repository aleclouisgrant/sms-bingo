#pragma once

#include <QWidget>
#include "BingoButtonGroup.h"
#include "Other/Player.h"
#include <QGridLayout>

class BingoBoard : public QWidget
{
	Q_OBJECT

public:
	BingoBoard(int boardSize, Player *player, QWidget *parent);
	~BingoBoard();

	Player* getPlayer();

	void PopulateBoard();
	void CheckBoard();
	void SelectSpace(int id);
	void SetSelectable(int id);
	void SelectRemoteSpace(int id);

signals:
	void SpaceSelected(int id);
	void Selectable(int id);

private:
	int m_boardSize;
	Player *m_player;

	QWidget *m_centralWidget;
	BingoButtonGroup *m_bingoButtonGroup;
	QGridLayout *m_bingoGrid;
};