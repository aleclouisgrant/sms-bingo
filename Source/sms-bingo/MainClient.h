#pragma once
#include <WS2tcpip.h>
#include <QMainWindow>

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <qlist.h>

#include "RoomWidget.h"

class MainClient : public QMainWindow
{
	Q_OBJECT

public:
	MainClient(QWidget *parent = Q_NULLPTR);
	~MainClient();


signals:
	void RequestAccepted();
	void Refresh(int roomSize);
	void IncomingRequest(char* username);

private:
	void LoginGUI();
	void RoomGUI();
	void DisplayAbout();
	void CreateRoom();
	void LogOut();
	void OpenGame();

	void CheckUsername();
	void MakeHomeWindow();
	void ConnectToServer();

	void ConnectToRemoteRoom(int port);
	void ConnectToLocalRoom();

	void Listen();

	QString m_username;

	SOCKET m_serverSocket;
	
	QList<RoomWidget *> m_roomArray;

	QVBoxLayout *m_layout;
	QVBoxLayout *m_roomLayout;
	QWidget *m_mainWidget;
	QFrame *m_roomWidget;

	QLineEdit *m_userInput;

	QWidget *m_logoWidget;
	QFrame *m_loginWidget;
	QWidget *m_buttonsWidget;

	QString *m_roomList;

private slots:
	void ShowRequest(char* username);
	void RefreshRoomList(int roomLength);
	void SendRequest(RoomWidget *roomWidget);
	void RejectRequest(char *username);
};
