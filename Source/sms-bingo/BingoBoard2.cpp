#include "BingoBoard2.h"
#include "BingoButton.h"
#include "BingoSpaceList.h"

#include <qdebug.h>

BingoBoard::BingoBoard(int boardSize, Player *player, QWidget *parent)
	: QWidget(parent)
{
	m_boardSize = boardSize;
	m_player = player;
	m_centralWidget = new QWidget(parent);

	m_bingoGrid = new QGridLayout(m_centralWidget);
	m_bingoButtonGroup = new BingoButtonGroup(m_centralWidget);

	for (int i = 0; i < m_boardSize; i++) {
		for (int j = 0; j < m_boardSize; j++) {
			int index = (i*m_boardSize) + j;
			QString btnText = QString::fromStdString("Option "
				+ std::to_string(index));
			BingoButton *btn = new BingoButton(m_centralWidget);
			btn->setText(btnText);
			btn->setMinimumSize(110);
			btn->setMaximumSize(110);

			m_bingoButtonGroup->addButton(btn);
			m_bingoButtonGroup->setId(btn, (i*m_boardSize) + j);
			m_bingoGrid->addWidget(btn, i, j);
		}
	}

	connect(m_bingoButtonGroup, QOverload<int>::of(
		&QButtonGroup::buttonClicked), this, &BingoBoard::SelectSpace);

	setLayout(m_bingoGrid);
}

BingoBoard::~BingoBoard()
{
}

Player * BingoBoard::getPlayer()
{
	return m_player;
}

void BingoBoard::PopulateBoard()
{
	BingoSpaceList *spaceList = new BingoSpaceList(m_boardSize, m_player);

	for (int i = 0; i < m_boardSize*m_boardSize; i++) {

		m_bingoButtonGroup->setSpace(i, spaceList->GetSpace(i));
	}
}

void BingoBoard::CheckBoard()
{
	for (int i = 0; i < m_boardSize*m_boardSize; i++) {
		if (m_bingoButtonGroup->checkSpace(i)) {
			emit Selectable(i);
		}
	}
}

void BingoBoard::SetSelectable(int id) 
{
	m_bingoButtonGroup->setSelectable(id);
}

void BingoBoard::SelectSpace(int id)
{
	m_bingoButtonGroup->selectSpace(id);
	emit SpaceSelected(id);
	qDebug() << "SpaceSelected emitted!! id is: " << id;
}
