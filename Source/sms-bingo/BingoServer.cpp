#include "BingoServer.h"
#include <thread>
#include <WS2tcpip.h>
#include <qdebug.h>

#pragma comment (lib, "ws2_32.lib")



BingoServer::BingoServer()
{
	qDebug() << "test test";
	std::thread scannerThread(&BingoServer::ServerStart, this);
	scannerThread.detach();
}


BingoServer::~BingoServer()
{
}


void BingoServer::ServerStart()
{
	WSADATA wsData; //place for winsock to write data to
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData); //initialize winsock
	if (wsOk != 0) {
		qDebug() << "Can't initialize winsock";
		return;
	}

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET) {
		qDebug() << "Can't create listening socket";
		return;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; //could also use inet_pton

	bind(listenSocket, (sockaddr *)&hint, sizeof(hint));

	listen(listenSocket, SOMAXCONN); //sets listening socket to listen

	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listenSocket, (sockaddr *)&client, &clientSize);
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
	qDebug() << "client connected on port: " << service;

	closesocket(listenSocket); //don't need the listening socket anymore
	const int bufferSize = 4096;
	char buf[bufferSize];

	
	while (true) {
		memset(buf, 0, bufferSize);

		int bytesReceived = recv(clientSocket, buf, bufferSize, 0);
		if (bytesReceived == SOCKET_ERROR) {
			qDebug() << "Error receiving data from client";
			break;
		}

		if (bytesReceived == 0) {
			//client disconnected;
			break;
		}

		send(clientSocket, buf, bytesReceived + 1, 0); //send data back to client
	}

	closesocket(clientSocket);
	WSACleanup();
}