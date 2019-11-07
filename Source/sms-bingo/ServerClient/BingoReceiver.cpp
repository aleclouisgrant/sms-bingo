#include "BingoReceiver.h"
#include <QObject>


BingoReceiver::BingoReceiver()
{

}


BingoReceiver::~BingoReceiver()
{
}

void BingoReceiver::DataReceived(int buttonId) 
{
	emit Received(buttonId);
}


/*
	Parse the setup data packet for information
*/
void BingoReceiver::SetupDataReceived(char *data)
{
	//sprintf(data, m_remoteUsername); //get the username of the remote player
	m_remoteUsername = data;
	
	emit SetupDataParsed();
}

char *BingoReceiver::GetRemoteUsername() 
{
	return m_remoteUsername;
}