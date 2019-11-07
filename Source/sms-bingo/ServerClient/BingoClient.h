#pragma once
#include <WS2tcpip.h>
#include "ServerClient/BingoReceiver.h"


class BingoClient
{
public:
	BingoClient(BingoReceiver *receiver);
	~BingoClient();

	void Send(int buttonId);
	void Disconnect();

private:
	void Start();
	int Connect();
	int GetSetupData();
	int SendSetupData();
	void Listen();

	BingoReceiver *m_receiver;
	SOCKET m_serverSocket;
};

