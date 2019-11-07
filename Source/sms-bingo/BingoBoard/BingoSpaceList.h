#pragma once
#include "BingoSpace.h"
#include "Other/Player.h"

class BingoSpaceList
{
public:
	BingoSpaceList(int boardSize, Player *player);
	~BingoSpaceList();

	BingoSpace* GetSpace(int id);

private:
	Player *m_player;

	BingoSpace **m_spaceArray;
};