#pragma once
#include <WS2tcpip.h>
#include <BingoReceiver.h>


class BingoClient
{
public:
	BingoClient(BingoReceiver *receiver);
	~BingoClient();

	void Send(int buttonId);

private:
	void Connect();
	void Disconnect();

	BingoReceiver *m_receiver;
	SOCKET m_serverSocket;
};

