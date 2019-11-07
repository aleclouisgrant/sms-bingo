#pragma once

#include <QMainWindow>
#include <tuple>
#include <WS2tcpip.h>

#define MAXROOMS 10
#define MAXCLIENTS 10

enum class ServerFlag : int {
	CheckUsername = (MAXCLIENTS + 1),
	AddRoom,
	DeleteRoom,
	RequestJoin,
	RequestAccept,
	UsernameAccept,
	UsernameReject,
	RefreshRooms
};

class MainServer : public QMainWindow
{
	Q_OBJECT

public:
	MainServer(QWidget *parent = Q_NULLPTR);
	~MainServer();

private:
	void Start();
	void DummyGUI();
	void RefreshAllRooms();
	void RefreshRooms(SOCKET outSock);
	void CheckUsername(SOCKET sock);
	void AddRoom(SOCKET sock);
	
	char *stou(SOCKET sock);
	SOCKET utos(char *username);

	SOCKET m_listenSocket;
	fd_set m_master;

	int m_roomList[MAXROOMS];
	int m_roomSize;

	std::map<int, char*> m_clientList;
	int m_clientSize;
};

