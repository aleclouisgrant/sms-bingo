#include "RoomWidget.h"
#include <Windows.h>


RoomWidget::RoomWidget(const int flag, QString username, QWidget *parent)
	: QFrame(parent)
{
	m_layout = new QHBoxLayout(this);
	m_username = username;

	QLabel *usernameLabel = new QLabel(m_username, this);

	m_layout->addWidget(usernameLabel, Qt::AlignLeft);

	if (flag == 0) {
		InitLocalRoom();
	}
	else if (flag == 1) {
		InitRemoteRoom();
	}

	setLayout(m_layout);
}

RoomWidget::~RoomWidget()
{
}

QString RoomWidget::GetUsername() { return m_username; }

void RoomWidget::InitLocalRoom()
{
	QPushButton *editButton = new QPushButton("Edit", this);
	QPushButton *deleteButton = new QPushButton("Delete", this);

	m_layout->addWidget(editButton, Qt::AlignRight);
	m_layout->addWidget(deleteButton, Qt::AlignRight);

	connect(editButton, &QPushButton::released, this, &RoomWidget::EditRoom);
	connect(deleteButton, &QPushButton::released, this, &RoomWidget::DeleteRoom);
}


void RoomWidget::InitRemoteRoom()
{
	QPushButton *joinButton = new QPushButton("Join", this);
	joinButton->setObjectName("JoinButton");

	m_layout->addWidget(joinButton, Qt::AlignRight);

	connect(joinButton, &QPushButton::released, this, &RoomWidget::JoinRoom);
}

void RoomWidget::JoinRoom()
{
	QPushButton *joinButton = this->findChild<QPushButton*>("JoinButton");
	joinButton->deleteLater();
	joinButton = nullptr;

	QLabel *waitLabel = new QLabel("...", this);

	m_layout->addWidget(waitLabel, Qt::AlignRight);

	emit RequestToJoin(this);
}

void RoomWidget::DeleteRoom()
{
	this->deleteLater();
}

void RoomWidget::EditRoom()
{
	return;
}
