#pragma once
#include <WS2tcpip.h>
#include <BingoReceiver.h>

class BingoServer
{
public:
	BingoServer(BingoReceiver *receiver);
	~BingoServer();

	void Send(int buttonId);

private:
	void Start();
	void Disconnect();

	BingoReceiver *m_receiver;
	SOCKET m_clientSocket;
};