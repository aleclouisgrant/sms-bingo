#include "BingoClient.h"
#include "BingoReceiver.h"
#include <WS2tcpip.h>
#include <string>
#include <thread>
#include <qdebug.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 4096

/*
 *
 */
BingoClient::BingoClient(BingoReceiver *receiver)
{
	m_receiver = receiver;

	qDebug() << "CLIENT: Ready";
	std::thread scannerThread(&BingoClient::Start, this);
	scannerThread.detach();
}


BingoClient::~BingoClient()
{
}

/*
 *
 */
void BingoClient::Start() {
	if (Connect() != 0) {
		return;
	}
	if (GetSetupData() != 0) {
		return;
	}
	if (SendSetupData() != 0) {
		return;
	}
	
	Listen();
}

/*
 *
 */
int BingoClient::Connect()
{
	std::string ipAddress = "127.0.0.1";	// ip address of the server
	int port = m_receiver->GetPort();		// listening port on the server

	WSAData wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &wsData); 	//initialize winsock
	if (wsResult != 0) {
		qDebug() << "CLIENT: Can't intialize WinSock";
		return -1;
	}

	m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_serverSocket == INVALID_SOCKET) {
		qDebug() << "CLIENT: Error creating socket";
		WSACleanup();
		return -1;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	int connectionResult = connect(m_serverSocket, (sockaddr *)&hint, sizeof(hint));
	if (connectionResult == SOCKET_ERROR) {
		qDebug() << "CLIENT: Cannot connect to server";
		closesocket(m_serverSocket);
		WSACleanup();
		return -1;
	}

	qDebug() << "CLIENT: Connected to server";
	return 0;
}

/*
 *
 */
int BingoClient::GetSetupData() 
{
	char buf[BUFFER_SIZE];
	memset(buf, 0, BUFFER_SIZE); //clear buffer

	int bytesReceived = recv(m_serverSocket, buf, sizeof(buf), 0);
	if (bytesReceived == SOCKET_ERROR) {
		qDebug() << "CLIENT: Error receiving data from client";
		return -1;
	}

	if (bytesReceived == 0) { //client disconnected
		return -1;
	}

	qDebug() << "CLIENT: Setup data received";
	m_receiver->SetupDataReceived(buf);

	return 0;
}

/*
 *
 */
int BingoClient::SendSetupData() 
{
	if (m_serverSocket == INVALID_SOCKET) {
		qDebug() << "CLIENT: Socket not created yet";
		return -1;
	}

	const char * data = "p2";

	char buf[BUFFER_SIZE];
	memcpy(&buf[0], data, sizeof(data));

	qDebug() << "CLIENT: Sending: " << buf;
	send(m_serverSocket, buf, BUFFER_SIZE, 0);

	return 0;
}


/*
 * receiving loop for the button id's in game
 */
void BingoClient::Listen() {	
	char buf[BUFFER_SIZE];

	while (true) { 
		memset(buf, 0, BUFFER_SIZE); //clear buffer

		int bytesReceived = recv(m_serverSocket, buf, sizeof(buf), 0);
		if (bytesReceived == SOCKET_ERROR) {
			qDebug() << "CLIENT: Error receiving data from client";
			break;
		}

		if (bytesReceived == 0) { //client disconnected
			break;
		}

		int id;
		sscanf(buf, "%d", &id);
		m_receiver->DataReceived(id);

		qDebug() << "CLIENT: Received data: " << buf;
	}

	closesocket(m_serverSocket);
	WSACleanup();
}

/*
 *
 */
void BingoClient::Send(int buttonId)
{
	if (m_serverSocket == INVALID_SOCKET) {
		qDebug() << "CLIENT: Socket not created yet";
		return;
	}

	char buf[BUFFER_SIZE];
	sprintf(buf, "%ld", buttonId);

	qDebug() << "CLIENT: Sending: " << buf;
	send(m_serverSocket, buf, BUFFER_SIZE, 0);
}

/*
 *
 */
void BingoClient::Disconnect() {
	closesocket(m_serverSocket);
	WSACleanup();
}