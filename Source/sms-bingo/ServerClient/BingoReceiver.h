#pragma once
#include <QObject>

class BingoReceiver : public QObject
{
	Q_OBJECT

public:
	BingoReceiver();
	~BingoReceiver();
	void DataReceived(int buttonId);
	void SetupDataReceived(char *data);

	char *GetRemoteUsername();

signals:
	void Received(int buttonId);
	void SetupDataParsed();

private:
	char* localUsername;
	char* m_remoteUsername;
	int boardSize;
	int ruleflags;
};
