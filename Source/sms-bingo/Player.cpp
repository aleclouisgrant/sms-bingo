#include "Player.h"


Player::Player(const char* usrtxt)
{
	username = usrtxt;
	elo = GetElo();

	score = 0;
}

Player::~Player()
{
}

//server call to get the elo of the player
int Player::GetElo() {
	return 1000;
}
