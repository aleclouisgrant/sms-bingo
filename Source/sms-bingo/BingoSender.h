#pragma once
#include <BingoServer.h>
#include <BingoClient.h>

class BingoSender
{
public:
	BingoSender(BingoServer *server);
	BingoSender(BingoClient *client);
	~BingoSender();

	void Send(int buttonId);

private:
	BingoServer *m_server;
	BingoClient *m_client;
};

