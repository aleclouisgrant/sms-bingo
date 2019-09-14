#pragma once
class Player
{
public:
	Player(const char* username);
	~Player();

	int score;

	const char* username;
	int elo;
	
private:
	int GetElo();

	int blueCoinTotal;
	/*
	int shineTotal;
	int shineTotalCheck;

	int shineTotalDP;
	int shineTotalBH;
	int shineTotalRH;
	int shineTotalGB;
	int shineTotalPP;
	int shineTotalSB;
	int shineTotalNB;
	int shineTotalPV;
	int shineTotalCM;
	int shineTotalAS;

	int redCoinShineTotal;
	int secretShineTotal;
	int hiddenShineTotal;

	int blueCoinShineTotal;
	int blueCoinTotal;
	int blueCoinTotalDP;
	int blueCoinTotalBH;
	int blueCoinTotalRH;
	int blueCoinTotalGB;
	int blueCoinTotalPP;
	int blueCoinTotalSB;
	int blueCoinTotalNB;
	int blueCoinTotalPV;
	int blueCoinTotalCM;

	int blueCoinLinkedTotal;
	int blueCoinBirdsTotal;
	int blueCoinMGraffittiTotal;
	int blueCoinFruitLadyTotal;

	int bossTotal;
	int uniqueBossTotal;
	int shadowMarioTotal;
	*/
};