#include "BingoSpaceList.h"

BingoSpaceList::BingoSpaceList(int boardSize, Player *player)
{
	m_player = player;

	BingoSpace *bs1 = new BingoSpace("Collect 1 Blue Coin");
	bs1->AddCondition(&m_player->m_blueCoinTotal, 1);
	BingoSpace *bs2 = new BingoSpace("Collect 2 Blue Coins");
	bs2->AddCondition(&m_player->m_blueCoinTotal, 2);
	BingoSpace *bs3 = new BingoSpace("Collect 3 Blue Coins");
	bs3->AddCondition(&m_player->m_blueCoinTotal, 3);

	BingoSpace *bs4 = new BingoSpace("Collect 3 Shines");
	bs4->AddCondition(&m_player->m_shineTotal, 3);
	BingoSpace *bs5 = new BingoSpace("Collect 4 Shines");
	bs5->AddCondition(&m_player->m_shineTotal, 4);
	BingoSpace *bs6 = new BingoSpace("Collect 5 Shines");
	bs6->AddCondition(&m_player->m_shineTotal, 5);


	BingoSpace *blank = new BingoSpace("--");
	blank->AddCondition(&m_player->m_shineTotal, 130);


	m_spaceArray = new BingoSpace*[boardSize*boardSize];
	m_spaceArray[0] = bs1;
	m_spaceArray[1] = bs2;
	m_spaceArray[2] = bs3;
	m_spaceArray[3] = bs4;
	m_spaceArray[4] = bs5;

	m_spaceArray[5] = blank;
	m_spaceArray[6] = blank;
	m_spaceArray[7] = blank;
	m_spaceArray[8] = blank;
	m_spaceArray[9] = blank;
	m_spaceArray[10] = blank;
	m_spaceArray[11] = blank;
	m_spaceArray[12] = blank;
	m_spaceArray[13] = blank;
	m_spaceArray[14] = blank;
	m_spaceArray[15] = blank;
	m_spaceArray[16] = blank;
	m_spaceArray[17] = blank;
	m_spaceArray[18] = blank;
	m_spaceArray[19] = blank;
	m_spaceArray[20] = blank;
	m_spaceArray[21] = blank;
	m_spaceArray[22] = blank;
	m_spaceArray[23] = blank;
	m_spaceArray[24] = blank;
}

BingoSpaceList::~BingoSpaceList()
{
}

BingoSpace* BingoSpaceList::GetSpace(int id) {
	return m_spaceArray[id];
}
