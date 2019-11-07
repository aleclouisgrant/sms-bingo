#pragma once
#include <WS2tcpip.h>
#include "ServerClient/BingoReceiver.h"

class BingoServer
{
public:
	BingoServer(BingoReceiver *receiver);
	~BingoServer();

	void Send(int buttonId);
	void Send(char *data);

private:
	void Start();
	int Connect();
	int SendSetupData();
	int GetSetupData();
	void Listen();
	void Disconnect();

	BingoReceiver *m_receiver;
	SOCKET m_clientSocket;
};