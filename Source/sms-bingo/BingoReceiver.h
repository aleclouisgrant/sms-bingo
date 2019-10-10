#pragma once
#include <QObject>

class BingoReceiver : public QObject
{
	Q_OBJECT

public:
	BingoReceiver();
	~BingoReceiver();
	void DataReceived(int buttonId);

signals:
	void Received(int buttonId);
};

