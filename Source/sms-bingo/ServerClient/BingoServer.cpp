#include "BingoServer.h"
#include "BingoReceiver.h"
#include <thread>
#include <WS2tcpip.h>
#include <qdebug.h>
#pragma comment (lib, "ws2_32.lib")

#define BUFFER_SIZE 4096

BingoServer::BingoServer(BingoReceiver *receiver)
{
	m_receiver = receiver;

	qDebug() << "HOST: Ready";
	std::thread serverThread(&BingoServer::Start, this);
	serverThread.detach();
}

BingoServer::~BingoServer()
{
}


void BingoServer::Start() 
{
	Connect();
	SendSetupData();
	GetSetupData();
	Listen();
}

int BingoServer::Connect()
{
	WSADATA wsData; //place for winsock to write data to
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData); //initialize winsock
	if (wsOk != 0) {
		qDebug() << "HOST: Can't initialize winsock";
		return -1;
	}

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET) {
		qDebug() << "HOST: Can't create listening socket";
		WSACleanup();
		return -1;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listenSocket, (sockaddr *)&hint, sizeof(hint));

	listen(listenSocket, SOMAXCONN); //sets listening socket to listen

	sockaddr_in client;
	int clientSize = sizeof(client);

	m_clientSocket = accept(listenSocket, (sockaddr *)&client, &clientSize);
	//should error check here

	char host[NI_MAXHOST];		//client's name
	char service[NI_MAXSERV];	//port that the client is connected through

	memset(host, 0, NI_MAXHOST);
	memset(service, 0, NI_MAXSERV);

	if (getnameinfo((sockaddr *)&client, clientSize, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {

	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		memcpy(service, (unsigned char *)ntohs(client.sin_port), NI_MAXSERV);
	}
	//should be connected at this point

	//just to display it for now
	qDebug() << "HOST: Client connected on port: " << service;

	closesocket(listenSocket); //don't need the listening socket anymore
	return 0;
}

int BingoServer::SendSetupData()
{
	char buf[BUFFER_SIZE];
	memset(buf, 0, BUFFER_SIZE);

	const char *data = "p1";

	memcpy(&buf[0], data, sizeof(data));

	qDebug() << "HOST: Sending set up data to client" << buf;
	send(m_clientSocket, buf, BUFFER_SIZE, 0);
	
	return 0;
}

int BingoServer::GetSetupData() 
{
	char buf[BUFFER_SIZE];

	//memset(buf, 0, BUFFER_SIZE); //clear buffer

	int bytesReceived = recv(m_clientSocket, buf, sizeof(buf), 0);
	if (bytesReceived == SOCKET_ERROR) {
		qDebug() << "HOST: Error receiving data from client";
		return -1;
	}

	if (bytesReceived == 0) { //client disconnected
		return -1;
	}

	qDebug() << "HOST: Setup data received";
	m_receiver->SetupDataReceived(buf);

	return 0;
}

void BingoServer::Listen() {
	char buf[BUFFER_SIZE];
	buf[0] = 1;

	while (true) { //receiving loop for the button id's in game
		memset(buf, 0, BUFFER_SIZE); //clear buffer

		int bytesReceived = recv(m_clientSocket, buf, sizeof(buf), 0);
		if (bytesReceived == SOCKET_ERROR) {
			qDebug() << "HOST: Error receiving data from client";
			break;
		}

		if (bytesReceived == 0) { //client disconnected
			break;
		}

		int id;
		sscanf(buf, "%d", &id);
		m_receiver->DataReceived(id);

		qDebug() << "HOST: Received data: " << buf;
	}

	closesocket(m_clientSocket);
	WSACleanup();
}

void BingoServer::Send(int buttonId) 
{
	if (m_clientSocket == INVALID_SOCKET) {
		qDebug() << "HOST: Socket not created yet";
		return;
	}

	char buf[BUFFER_SIZE];
	sprintf(buf, "%ld", buttonId);

	qDebug() << "HOST: Sending: " << buf;
	send(m_clientSocket, buf, sizeof(buf), 0);
}

void BingoServer::Disconnect()
{
	closesocket(m_clientSocket);
	WSACleanup();
}