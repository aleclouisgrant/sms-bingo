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
