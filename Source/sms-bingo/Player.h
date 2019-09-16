#pragma once

class Player
{
public:
	Player(const char* username);
	~Player();

	int score;

	const char* username;
	int elo;

	const char* getUsername();
	int getElo();
	int getScore();

	void SetValue(int *var, int value);

	int getShineTotal();
	int getShineTotalCheck();

	int getShineTotalDP();
	int getShineTotalBH();
	int getShineTotalRH();
	int getShineTotalGB();
	int getShineTotalPP();
	int getShineTotalSB();
	int getShineTotalNB();
	int getshineTotalPV();
	int getShineTotalCM();
	int getShineTotalAS();

	int getRedCoinShineTotal();
	int getSecretShineTotal();
	int getHiddenShineTotal();

	int getBlueCoinShineTotal();
	int getBlueCoinTotal();
	int getBlueCoinTotalDP();
	int getBlueCoinTotalBH();
	int getBlueCoinTotalRH();
	int getBlueCoinTotalGB();
	int getBlueCoinTotalPP();
	int getBlueCoinTotalSB();
	int getBlueCoinTotalNB();
	int getBlueCoinTotalPV();
	int getBlueCoinTotalCM();

	int getBlueCoinLinkedTotal();
	int getBlueCoinBirdsTotal();
	int getBlueCoinMGraffittiTotal();
	int getBlueCoinFruitLadyTotal();

	int getBossTotal();
	int getUniqueBossTotal();
	int getShadowMarioTotal();



	int m_shineTotal;
	int m_shineTotalCheck;

	int m_shineTotalDP;
	int m_shineTotalBH;
	int m_shineTotalRH;
	int m_shineTotalGB;
	int m_shineTotalPP;
	int m_shineTotalSB;
	int m_shineTotalNB;
	int m_shineTotalPV;
	int m_shineTotalCM;
	int m_shineTotalAS;

	int m_redCoinShineTotal;
	int m_secretShineTotal;
	int m_hiddenShineTotal;

	int m_blueCoinShineTotal;
	int m_blueCoinTotal;
	int m_blueCoinTotalDP;
	int m_blueCoinTotalBH;
	int m_blueCoinTotalRH;
	int m_blueCoinTotalGB;
	int m_blueCoinTotalPP;
	int m_blueCoinTotalSB;
	int m_blueCoinTotalNB;
	int m_blueCoinTotalPV;
	int m_blueCoinTotalCM;

	int m_blueCoinLinkedTotal;
	int m_blueCoinBirdsTotal;
	int m_blueCoinMGraffittiTotal;
	int m_blueCoinFruitLadyTotal;

	int m_bossTotal;
	int m_uniqueBossTotal;
	int m_shadowMarioTotal;

private:
	int GetElo();
	
};