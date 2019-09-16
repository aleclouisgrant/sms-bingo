#include "BingoSpaceList.h"

BingoSpaceList::BingoSpaceList(int boardSize, Player *player)
{
	m_player = player;

	BingoSpace *bs1 = new BingoSpace("Collect 2 Blue Coins");
	bs1->AddCondition(&m_player->m_blueCoinTotal, 2);

	BingoSpace *bs2 = new BingoSpace("Collect 2 Shines");
	bs2->AddCondition(&m_player->m_shineTotal, 2);

	BingoSpace *bs3 = new BingoSpace("Collect 3 Blue Coins");
	bs3->AddCondition(&m_player->m_blueCoinTotal, 3);

	m_spaceArray = new BingoSpace*[boardSize*boardSize];
	m_spaceArray[0] = bs1;
	m_spaceArray[1] = bs2;
	m_spaceArray[2] = bs3;

	m_spaceArray[3] = bs2;
	m_spaceArray[4] = bs2;
	m_spaceArray[5] = bs2;
	m_spaceArray[6] = bs2;
	m_spaceArray[7] = bs2;
	m_spaceArray[8] = bs2;
	m_spaceArray[9] = bs2;
	m_spaceArray[10] = bs2;
	m_spaceArray[11] = bs2;
	m_spaceArray[12] = bs2;
	m_spaceArray[13] = bs2;
	m_spaceArray[14] = bs2;
	m_spaceArray[15] = bs2;
	m_spaceArray[16] = bs2;
	m_spaceArray[17] = bs2;
	m_spaceArray[18] = bs2;
	m_spaceArray[19] = bs2;
	m_spaceArray[20] = bs2;
	m_spaceArray[21] = bs2;
	m_spaceArray[22] = bs2;
	m_spaceArray[23] = bs2;
	m_spaceArray[24] = bs2;
}

BingoSpaceList::~BingoSpaceList()
{
}

BingoSpace* BingoSpaceList::GetSpace(int id) {
	return m_spaceArray[id];
}
