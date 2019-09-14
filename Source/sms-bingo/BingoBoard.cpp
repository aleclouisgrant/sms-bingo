#include "BingoBoard.h"
#include "BingoButton.h"

BingoBoard::BingoBoard(int boardSize, QWidget *parent) 
	: QWidget(parent)
{
	m_boardSize = boardSize;
	m_centralWidget = new QWidget(parent);

	m_bingoGrid = new QGridLayout(m_centralWidget);
	m_bingoButtonGroup = new BingoButtonGroup(this);

	for (int i = 0; i < m_boardSize; i++) {
		for (int j = 0; j < m_boardSize; j++) {
			int index = (i*m_boardSize) + j;
			QString btnText = QString::fromStdString("Option "
				+ std::to_string(index));
			BingoButton *btn = new BingoButton(this);
			btn->setText(btnText);
			btn->setMinimumSize(110);
			btn->setMaximumSize(110);

			m_bingoButtonGroup->addButton(btn);
			m_bingoButtonGroup->setId(btn, (i*m_boardSize) + j);
			m_bingoGrid->addWidget(btn, i, j);
		}
	}

	setLayout(m_bingoGrid);
}


BingoBoard::~BingoBoard()
{
}

void BingoBoard::PopulateBoard()
{
	QString spaceArray[] = {
		//"Collect 6 Secret Shines",
		"Collect 17 Blue Coins",
		"Collect 1 Hidden Shine in Ricco Harbor",
		"15 Blue Coins from M Graffiti",
		"55 Total Blue Coins",
		"Pinna Park Episode 8 Shine",
		"Ride Yoshi in Pinna Park",
		"Collect 2 Hidden Shines in Pianta Village",
		"20 Blue Coins from Noki Bay",
		"100 Coin Shine from Ricco Harbor",
		//"100 Coin Shine from Gelato Beach",
		"Collect 4 Shines",
		"30 Total Blue Coins",
		"Pinna 3 Hoverless",
		"Bianco Hills Episode 8 Shine",
		"11 Shines from Delfino Plaza",
		"Defeat Shadow Mario in Gelato Beach",
		"Defeat the Eel of Noki Bay",
		"Lily Pad Shine",
		"5 Linked Blue Coin Pairs",
		"20 Blue Coins from Pinna Park",
		"10 Blue Coins from Noki Bay",
		"Collect 1 Hidden Shine from Noki Bay",
		"25 Lives",
		"Beat Piantissimo in Noki Bay",
		"2 Shadow Marios",
		"3 Fruit Lady Blue Coins"
	};

	//BingoSpaceList space_list = new BingoSpaceList(m_boardSize);

	for (int i = 0; i < m_boardSize*m_boardSize; i++) {
		m_bingoButtonGroup->setText(i, spaceArray[i]);
		//m_bingoButtonGroup->SetSpace(i, space_list[i]);
	}

}

