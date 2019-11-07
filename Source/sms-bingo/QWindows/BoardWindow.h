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
#include "BingoBoard/BingoButtonGroup.h"
#include "Other/Player.h"
#include "MemoryScanner/MemoryScanner.h"
#include "BingoBoard/BingoBoard2.h"
#include "ServerClient/BingoSender.h"
#include "ServerClient/BingoReceiver.h"

class BoardWindow : public QMainWindow
{
	Q_OBJECT

public:
	BoardWindow(BingoSender *sender, BingoReceiver *receiver, QWidget *parent = Q_NULLPTR);
	~BoardWindow();

private slots:
	void SelectLocalSpace(int id);
	void SetSelectable(int id);
	void SelectRemoteSpace(int id);

private:
	void MakePlayers();
	void HookToDolphin();
	void MakeWidgets();
	void DisplayGameView();
	void StylizeText();
	void ConnectButtons();
	void ReadStyleSheets();

	MemoryScanner *m_memscanner;
	BingoSender *m_sender;
	BingoReceiver *m_receiver;

	int boardSize;
	double aspect_ratio;

	QString m_boardSS;

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
