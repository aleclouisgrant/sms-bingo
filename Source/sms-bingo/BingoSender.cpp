#include "BingoSender.h"
#include <qdebug.h>


BingoSender::BingoSender(BingoServer *server)
{
	m_server = server;
	m_client = NULL;
}

BingoSender::BingoSender(BingoClient *client)
{
	m_client = client;
	m_server = NULL;
}


BingoSender::~BingoSender()
{
}

void BingoSender::Send(int buttonId)
{
	if (m_server == NULL) { //client has been passed to Sender
		m_client->Send(buttonId);
	}
	else if (m_client == NULL) { //server has been passed to Sender
		m_server->Send(buttonId);
	}
	else { //something went wrong
		qDebug() << "Error: no server or client set in Sender";
	}

	return;
}