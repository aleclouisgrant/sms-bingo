#include "HomeWindow.h"
#include "ServerClient/BingoServer.h"
#include "ServerClient/BingoClient.h"
#include "ServerClient/BingoSender.h"
#include "QWindows/BoardWindow.h"
#include <qdebug.h>
#include <qpushbutton.h>

HomeWindow::HomeWindow(QWidget *parent)
	: QMainWindow(parent)
{
	m_username = "local player";

	m_centralWidget = new QWidget;
	m_centralWidget->setObjectName("CentralWidget");
	m_optionsWidget = new QWidget;
	m_optionsWidget->setObjectName("OptionsWidget");

	m_centralLayout = new QVBoxLayout(this);
	m_optionsLayout = new QVBoxLayout(this);

	m_optionsLabel = new QLabel("Options", this);
	m_optionsLabel->setObjectName("OptionsLabel");
	m_lockoutLabel = new QLabel("Lockout:", this);
	m_lockoutLabel->setObjectName("LockoutLabel");
	m_connect5Label = new QLabel("Connect 5:", this);
	m_connect5Label->setObjectName("Connect5Label");
	m_allowDelayClickLabel = new QLabel("Allow delayed click:", this);
	m_allowDelayClickLabel->setObjectName("AllowDelayClickLabel");
	
	m_bingoButton = new BingoButton(this);
	m_bingoButton->setMinimumSize(100);
	m_bingoButton->setMaximumSize(100);
	m_bingoButton->setText("HELLO LETS SEE IF THIS BAD BOY WORD WRAPS OR NOT");

	m_hostButton = new QPushButton("HOST", this);
	m_clientButton = new QPushButton("CLIENT", this);

	m_optionsWidget->setLayout(m_optionsLayout);
	m_optionsLayout->addWidget(m_optionsLabel);
	m_optionsLayout->addWidget(m_lockoutLabel);
	m_optionsLayout->addWidget(m_connect5Label);
	m_optionsLayout->addWidget(m_allowDelayClickLabel);
	m_optionsLayout->addWidget(m_bingoButton);
	m_optionsLayout->addWidget(m_hostButton);
	m_optionsLayout->addWidget(m_clientButton);

	m_centralWidget->setLayout(m_centralLayout);
	m_centralLayout->addWidget(m_optionsWidget);
	setCentralWidget(m_centralWidget);


	m_receiver = new BingoReceiver(0);
	ConnectButtons();
}

HomeWindow::~HomeWindow()
{
}

void HomeWindow::ConnectButtons()
{
	connect(m_hostButton, &QPushButton::released, this,
		&HomeWindow::MakeHost);
	connect(m_clientButton, &QPushButton::released, this,
		&HomeWindow::MakeClient);
	connect(m_receiver, &BingoReceiver::SetupDataParsed, this,
		&HomeWindow::MakeBoardWindow);
}

void HomeWindow::MakeHost()
{
	qDebug() << "Making host server...";

	BingoServer *host = new BingoServer(m_receiver);
	m_sender = new BingoSender(host);
	MakeRoomWidget();
}

void HomeWindow::MakeClient()
{
	qDebug() << "Making client...";

	BingoClient *client = new BingoClient(m_receiver);
	m_sender = new BingoSender(client);
}


void HomeWindow::MakeRoomWidget()
{
	QVBoxLayout *roomLayout = new QVBoxLayout(this);
	QWidget *roomWidget = new QWidget(this);

	QLabel *roomOwner = new QLabel("LOCAL PLAYER", this);
	QLabel *roomOptions = new QLabel("Lockout, Connect 5", this);
	QPushButton *editButton = new QPushButton("Edit", this);
	QPushButton *deleteButton = new QPushButton("Delete", this);

	//connect(editButton, &QPushButton::released, this, &HomeWindow::EditRoomOptions);
	//connect(deleteButton, &QPushButton::released, this, &HomeWindow::DeleteRoom);

	roomLayout->addWidget(roomOwner);
	roomLayout->addWidget(roomOptions);
	roomLayout->addWidget(editButton);
	roomLayout->addWidget(deleteButton);

	roomWidget->setLayout(roomLayout);
	m_centralLayout->addWidget(roomWidget);
}

void HomeWindow::MakeBoardWindow()
{
	qDebug() << "Making board window";

	BoardWindow *window = new BoardWindow(m_sender, m_receiver);
	window->show();
	//this->hide();
}


void HomeWindow::UnhideWindow()
{
	this->show();
}