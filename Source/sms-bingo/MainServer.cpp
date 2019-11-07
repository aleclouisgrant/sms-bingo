#include "MainServer.h"
#include <WS2tcpip.h>
#include <qdebug.h>

#include <thread>
#include "MainClient.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#pragma comment (lib, "ws2_32.lib")

#define PORT_NUMBER 54000
#define BUFFER_SIZE 4096

/*

This is just a dummy of the main server software that will be 
run on the game's dedicated static server

*/
MainServer::MainServer(QWidget *parent)
	: QMainWindow(parent)
{
	qDebug() << "MAINSERVER: starting.....";
	for (int i = 0; i < MAXROOMS; i++) {
		m_roomList[i] = 0;
	}

	m_roomSize = 0;
	m_clientSize = 0;

	DummyGUI();

	std::thread t(&MainServer::Start, this);
	t.detach();
}

MainServer::~MainServer()
{
}

void MainServer::DummyGUI()
{
	QWidget *widget = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout(this);
	QLabel *label = new QLabel("MAIN SERVER", this);

	layout->addWidget(label);
	widget->setLayout(layout);

	setCentralWidget(widget);
}


void MainServer::Start()
{
	WSADATA wsData; //place for winsock to write data to
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData); //initialize winsock
	if (wsOk != 0) {
		qDebug() << "HOST: Can't initialize winsock";
		return;
	}

	m_listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_listenSocket == INVALID_SOCKET) {
		qDebug() << "HOST: Can't create listening socket";
		WSACleanup();
		return;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(PORT_NUMBER);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(m_listenSocket, (sockaddr *)&hint, sizeof(hint));

	listen(m_listenSocket, SOMAXCONN); //sets listening socket to listen

	FD_ZERO(&m_master);
	FD_SET(m_listenSocket, &m_master);

	while (true) {
		fd_set copy = m_master; //select is destructive which is why we need to make a copy
		int count = select(0, &copy, nullptr, nullptr, nullptr);

		for (int i = 0; i < count; i++) {
			SOCKET sock = copy.fd_array[i];
			if (sock == m_listenSocket) { // client is attempting to connect
				SOCKET clientSocket = accept(m_listenSocket, nullptr, nullptr);
				FD_SET(clientSocket, &m_master);

				char *val = NULL;
				m_clientList.insert(std::make_pair((int)clientSocket, val));
				qDebug() << "MAINSERVER: Connected to client";
			}
			else { // client is attempting to send a message
				char buf[BUFFER_SIZE];
				memset(buf, 0, BUFFER_SIZE);
				
				int bytesReceived = recv(sock, buf, BUFFER_SIZE, 0);
				
				if (bytesReceived <= 0) { //drop the client
					closesocket(sock);
					FD_CLR(sock, &m_master);
				}
				else {
					ServerFlag flag;
					sscanf(buf, "%d", &flag);

					if (flag == ServerFlag::CheckUsername) {
						CheckUsername(sock);
					}
					else if (flag == ServerFlag::AddRoom) { //request to add room
						AddRoom(sock);
					}
					else if (flag == ServerFlag::DeleteRoom) { //request to delete room
						qDebug() << "MAINSERVER: Request to delete room received by " << sock << ".";
						
						for (int i = 0; i < m_roomSize; i++) {
							if (m_roomList[i] == sock) {
								m_roomList[i] = 0;
								qDebug() << "MAINSERVER: Room deleted.";
								m_roomSize = m_roomSize - 1;
								break;
							}
						}
					}
					else if (flag == ServerFlag::RequestJoin) { //request to connect to room
						RequestJoin(sock);
					}
					else if (flag == ServerFlag::RefreshRooms) {
						RefreshRooms(sock);
					}
				}
			}
		}

	}

	WSACleanup();
}

void MainServer::CheckUsername(SOCKET sock)
{
	char buf[BUFFER_SIZE];
	memset(buf, 0, BUFFER_SIZE);
	int bytesReceived = recv(sock, buf, BUFFER_SIZE, 0);
	char *username = new char[BUFFER_SIZE];
	sscanf(buf, "%s", username);

	qDebug() << "MAINSERVER: Checking username (" << username << ").";

	if (utos(username) != 0) {
		qDebug() << "MAINSERVER: Username " << username << " already in use.";
		memset(buf, 0, BUFFER_SIZE);
		sprintf(buf, "%d", ServerFlag::UsernameReject);
		send(sock, buf, BUFFER_SIZE, 0);
	}
	else {
		m_clientList[sock] = username;
		qDebug() <<
			"MAINSERVER: Username registered to client list <" << sock <<
			", " << username << ">.";
		m_clientSize++;

		memset(buf, 0, BUFFER_SIZE);
		sprintf(buf, "%d", ServerFlag::UsernameAccept);
		send(sock, buf, BUFFER_SIZE, 0);
	}
}

void MainServer::AddRoom(SOCKET sock)
{
	qDebug() << "MAINSERVER: Request to add room received by " << stou(sock);

	if (m_roomSize < MAXROOMS) {
		for (int i = 0; i <= m_roomSize; i++) {
			qDebug() << "MAINSERVER: m_roomList[]: " << m_roomList[i];
			if (m_roomList[i] == 0) {
				m_roomList[i] = sock;
				m_roomSize++;
				qDebug() << "MAINSERVER: Room added by " << stou(sock) << ".";
				RefreshAllRooms();
				break;
			}
		}
	}
	else {
		qDebug() << "MAINSERVER: Room list is full";
	}
}

void MainServer::RefreshAllRooms()
{
	qDebug() << "MAINSERVER: Refreshing room lists for all clients.";
	qDebug() << "MAINSERVER: Room List[" << m_roomSize << "]: ";
	for (int i = 0; i < m_roomSize; i++) {
		qDebug() << m_roomList[i];
	}

	for (int i = 0; i < m_master.fd_count; i++) {
		SOCKET outSock = m_master.fd_array[i];
		if (outSock != m_listenSocket && stou(outSock) != NULL ) {
			RefreshRooms(outSock);
		}
	}
}

void MainServer::RefreshRooms(SOCKET outSock)
{
	qDebug() << "MAINSERVER: Refreshing room list for client: " << stou(outSock);
	char buf[BUFFER_SIZE];
	memset(buf, 0, BUFFER_SIZE);
	sprintf(buf, "%d", ServerFlag::RefreshRooms);
	send(outSock, buf, sizeof(buf), 0);

	//send the number of rooms
	qDebug() << "MAINSERVER: Roomsize: " << m_roomSize;
	memset(buf, 0, BUFFER_SIZE);
	sprintf(buf, "%d", m_roomSize);
	send(outSock, buf, sizeof(buf), 0);

	//send the usernames of each room
	for (int i = 0; i < m_roomSize; i++) {
		memset(buf, 0, BUFFER_SIZE);
		sprintf(buf, "%s", m_clientList.find(m_roomList[i])->second);

		qDebug() << "MAINSERVER: " << buf;
		send(outSock, buf, sizeof(buf), 0);
	}
}

void MainServer::RequestJoin(SOCKET client)
{
	char buf[BUFFER_SIZE];
	memset(buf, 0, BUFFER_SIZE);

	int bytesReceived = recv(client, buf, BUFFER_SIZE, 0);
	char host[BUFFER_SIZE]; //username of host from client
	sscanf(buf, "%s", host); 
	qDebug() << "MAINSERVER: Request to connect to " << host << " by " << stou(client);
	
	//send the request flag to the host
	memset(buf, 0, BUFFER_SIZE);
	sprintf(buf, "%d", ServerFlag::RequestJoin);
	send(utos(host), buf, BUFFER_SIZE, 0);
	qDebug() << "MAINSERVER: Sending request flag to " << host << " from " << stou(client);
	
	//send the username of client to host
	memset(buf, 0, BUFFER_SIZE);
	sprintf(buf, "%s", stou(client));
	send(utos(host), buf, BUFFER_SIZE, 0);
	qDebug() << "MAINSERVER: Sending username to " << host << " from " << stou(client);

	//get whether the host accepted or not
	memset(buf, 0, BUFFER_SIZE);
	bytesReceived = recv(client, buf, BUFFER_SIZE, 0);
	ServerFlag flag;
	sscanf(buf, "%d", &flag);
	qDebug() << "MAINSERVER: Host (" << host << ") sent answer: " << (int)flag;

	//send the flag to the client
	send(client, buf, BUFFER_SIZE, 0);

	if (flag == ServerFlag::RequestAccept) {
		//send the host's port to the client so they can connect
		sockaddr_in *addr;
		int *len;
		getsockname(utos(host), (sockaddr *)addr, len);

		memset(buf, 0, BUFFER_SIZE);
		sprintf(buf, "%d", ntohs(addr->sin_port));
		send(client, buf, BUFFER_SIZE, 0);
		qDebug() << "MAINSERVER: Sending port (" << ntohs(addr->sin_port) 
			<< ") to client ( " << stou(client) << ")";
	}
}

char *MainServer::stou(SOCKET sock) { return m_clientList.find(sock)->second; }
SOCKET MainServer::utos(char *username) {
	auto it = m_clientList.begin();
	while (it != m_clientList.end()) {
		if (it->second != NULL) { 
			if (strcmp(it->second, username) == 0) {
				return (SOCKET)it->first;
			}
		}
		it++;
	}
	return 0;
}