#include "BingoClient.h"
#include <WS2tcpip.h>
#include <string>
#include <thread>
#include <qdebug.h>
#include <BingoReceiver.h>
#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 4096

BingoClient::BingoClient(BingoReceiver *receiver)
{
	m_receiver = receiver;

	qDebug() << "Client being made";
	std::thread scannerThread(&BingoClient::Connect, this);
	scannerThread.detach();
}


BingoClient::~BingoClient()
{
}


void BingoClient::Connect()
{
	std::string ipAddress = "127.0.0.1";	// ip address of the server
	int port = 54000;						// listening port on the server

	WSAData wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &wsData); 	//initialize winsock
	if (wsResult != 0) {
		qDebug() << "Can't intialize WinSock";
		return;
	}

	m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_serverSocket == INVALID_SOCKET) {
		qDebug() << "Error creating socket";
		WSACleanup();
		return;
	}
	
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	int connectionResult = connect(m_serverSocket, (sockaddr *)&hint, sizeof(hint));
	if (connectionResult == SOCKET_ERROR) {
		qDebug() << "Cannot connect to server";
		closesocket(m_serverSocket);
		WSACleanup();
		return;
	}

	// this is the receiving loop
	char buf[BUFFER_SIZE];

	while (true) {
		memset(buf, 0, BUFFER_SIZE); //clear buffer

		int bytesReceived = recv(m_serverSocket, buf, sizeof(buf), 0);
		if (bytesReceived == SOCKET_ERROR) {
			qDebug() << "Error receiving data from client";
			break;
		}

		if (bytesReceived == 0) { //client disconnected
			break;
		}

		int id;
		sscanf(buf, "%d", &id);
		m_receiver->DataReceived(id);
		qDebug() << "Received data: " << buf;
	}

	closesocket(m_serverSocket);
	WSACleanup();
}

void BingoClient::Send(int buttonId)
{
	if (m_serverSocket == INVALID_SOCKET) {
		qDebug() << "Socket not created yet";
		return;
	}

	char buf[BUFFER_SIZE];
	sprintf(buf, "%ld", buttonId);

	qDebug() << "Sending: " << buf;
	send(m_serverSocket, buf, BUFFER_SIZE, 0);
}

void BingoClient::Disconnect() {
	closesocket(m_serverSocket);
	WSACleanup();
}