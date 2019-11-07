#pragma once

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>


class RoomWidget : public QFrame
{
	Q_OBJECT

public:
	RoomWidget(const int flag, QString username, QWidget *parent);
	~RoomWidget();

	QString GetUsername();

signals:
	void RequestToJoin(RoomWidget *roomWidget);

private:
	void InitLocalRoom();
	void InitRemoteRoom();

	void JoinRoom();
	void DeleteRoom();
	void EditRoom();

	QHBoxLayout *m_layout;

	QString m_username;

};
