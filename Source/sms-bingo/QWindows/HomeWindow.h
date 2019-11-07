#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include "BingoBoard/BingoButton.h"
#include "ServerClient/BingoSender.h"
#include "ServerClient/BingoReceiver.h"
#include <qpushbutton.h>

class HomeWindow : public QMainWindow
{
	Q_OBJECT

public:
	HomeWindow(QWidget *parent = Q_NULLPTR);
	~HomeWindow();
	void UnhideWindow();

private:
	void ConnectButtons();
	void MakeHost();
	void MakeClient();
	void MakeBoardWindow();
	void MakeRoomWidget();
	
	BingoReceiver *m_receiver;
	BingoSender *m_sender;

	QWidget* m_centralWidget;
	QWidget* m_optionsWidget;

	QVBoxLayout* m_centralLayout;
	QVBoxLayout* m_optionsLayout;
	
	QLabel* m_optionsLabel;
	QLabel* m_lockoutLabel;
	QLabel* m_connect5Label;
	QLabel* m_allowDelayClickLabel;
	BingoButton* m_bingoButton;

	QPushButton *m_hostButton;
	QPushButton *m_clientButton;

	const char *m_username;
};
