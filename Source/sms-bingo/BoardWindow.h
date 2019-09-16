#pragma once

#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QScreen>
#include <QButtonGroup>
#include "BingoButtonGroup.h"
#include "Player.h"
#include <MemoryScanner/MemoryScanner.h>
#include <BingoBoard2.h>

class BoardWindow : public QMainWindow
{
	Q_OBJECT

public:
	BoardWindow(QWidget *parent = Q_NULLPTR);
	~BoardWindow();
	void MakePlayers();
	void HookToDolphin();
	void MakeWidgets();
	void DisplayGameView();
	void StylizeText();
	void ConnectButtons();
	void ReadStyleSheets();

private slots:
	void ToggleView();
	void SelectSpace();
	void SetSelectable(int id);

private:
	MemoryScanner *m_memscanner;
	int boardSize;
	double aspect_ratio;

	QString m_boardSS;
	//QString m_selectableSpaceSS;
	//QString m_selectedSpaceSS;

	QScreen *screen;
	QRect screenGeometry;

	Player *localPlayer;
	Player *remotePlayer;

	QWidget *m_centralWidget;
	QWidget *m_menuWidget;
	QWidget *m_bingoWidget;

	QVBoxLayout *mainLayout;
	QHBoxLayout *m_menuLayout;

	QLabel *m_logoLabel_SuperMario;
	QLabel *m_logoLabel_Sunshine;
	QLabel *m_logoLabel_Bingo;
	QLabel *m_timerLabel;
	QLabel *m_localPlayerLabel;
	QLabel *m_remotePlayerLabel;
	BingoButtonGroup *m_bingoButtonGroup;

	BingoBoard *m_bingoBoard;

	QPushButton *m_rulesButton;
	QPushButton *m_optionsButton;
	QPushButton *m_forfeitButton;
};
