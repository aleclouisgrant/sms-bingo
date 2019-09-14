#pragma once

#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include "Player.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QScreen>
#include <QButtonGroup>

class MainWindow : public QMainWindow
{
	Q_OBJECT

	enum View { GAMEVIEW, BOARDVIEW };

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();
	void MakeWidgets();
	void DisplayGameView();
	void DisplayBoardView();
	void RemoveWidgets();
	void MakeBingoBoard();
	void MakePlayers();
	void StylizeText();
	void ConnectButtons();
	void ReadStyleSheets();

private slots:
	void ToggleView();
	void SelectSpace(int id);


private:
	int boardSize;
	double aspect_ratio;

	QString gameViewSS;
	QString boardViewSS;
	QString selectSpaceSS;
	QScreen *screen;
	QRect screenGeometry;

	Player *localPlayer;
	Player *remotePlayer;
	View m_view;
	
	QWidget *m_centralWidget;
	QWidget *m_rightWidget;
	QWidget *m_bingoWidget;

	QHBoxLayout *m_hLayout;
	QVBoxLayout *m_rightLayout;
	QGridLayout *m_bingoGrid;

	QFrame *m_frame;
	QLabel *m_logoLabel_SuperMario;
	QLabel *m_logoLabel_Sunshine;
	QLabel *m_logoLabel_Bingo;
	QLabel *m_timerLabel;
	QLabel *m_localPlayerLabel;
	QLabel *m_remotePlayerLabel;
	QPushButton *m_button;
	QButtonGroup *m_bingoButtonGroup;
};
