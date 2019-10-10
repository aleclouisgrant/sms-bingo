#include "BoardWindow.h"
#include <qguiapplication.h>
#include <QGraphicsEffect>
#include <QSizePolicy>
#include <QFile>
#include <QResizeEvent>
#include "BingoButton.h"
#include <DolphinProcess/DolphinAccessor.h>
#include <MemoryScanner/MemoryScanner.h>
#include <Player.h>


BoardWindow::BoardWindow(BingoSender *sender, BingoReceiver *receiver, QWidget *parent)
	: QMainWindow(parent)
{
	m_sender = sender;
	m_receiver = receiver;

	boardSize = 5;
	aspect_ratio = 1.2121212;

	screen = QGuiApplication::primaryScreen();
	screenGeometry = screen->geometry();

	MakePlayers();
	ReadStyleSheets();
	MakeWidgets();
	HookToDolphin();
	DisplayGameView();
	ConnectButtons();
}

BoardWindow::~BoardWindow()
{

}

void BoardWindow::MakePlayers()
{
	localPlayer = new Player("local player");
	remotePlayer = new Player("remote player");
}

void BoardWindow::HookToDolphin() 
{
	DolphinComm::DolphinAccessor::hook();
	if (DolphinComm::DolphinAccessor::getStatus() ==
		DolphinComm::DolphinAccessor::DolphinStatus::hooked) {
		m_memscanner = new MemoryScanner(m_bingoBoard);
		m_memscanner->Start();
	}
}

void BoardWindow::ReadStyleSheets()
{
	QFile file(":/QSS/board.qss");
	file.open(QFile::ReadOnly);
	m_boardSS = QLatin1String(file.readAll());
	file.close();

	/*
	QFile selectableSpaceFile(":/QSS/selectedSpace.qss");
	selectableSpaceFile.open(QFile::ReadOnly);
	m_selectableSpaceSS = QLatin1String(selectableSpaceFile.readAll());
	selectableSpaceFile.close();
	*/
}

void BoardWindow::MakeWidgets()
{
	m_centralWidget = new QWidget;
	m_centralWidget->setObjectName("CentralWidget");
	m_menuWidget = new QWidget;
	m_menuWidget->setObjectName("RightWidget");
	m_bingoWidget = new QWidget;
	m_bingoWidget->setObjectName("BingoWidget");
	m_menuWidget = new QWidget;
	m_menuWidget->setObjectName("MenuWidget");

	mainLayout = new QVBoxLayout(this);
	m_menuLayout = new QHBoxLayout(this);

	m_logoLabel_SuperMario = new QLabel("Super Mario", this);
	m_logoLabel_SuperMario->setObjectName("LogoLabel_SuperMario");
	m_logoLabel_Sunshine = new QLabel("Sunshine", this);
	m_logoLabel_Sunshine->setObjectName("LogoLabel_Sunshine");
	m_logoLabel_Bingo = new QLabel("Bingo", this);
	m_logoLabel_Bingo->setObjectName("LogoLabel_Bingo");

	m_timerLabel = new QLabel("0:00:00.0", this);
	m_timerLabel->setObjectName("TimerLabel");
	m_localPlayerLabel = new QLabel(localPlayer->username +
		QString::fromStdString(" x ") + QString::number(localPlayer->score),
		this);
	m_localPlayerLabel->setObjectName("LocalPlayerLabel");
	m_remotePlayerLabel = new QLabel(remotePlayer->username +
		QString::fromStdString(" x ") + QString::number(remotePlayer->score),
		this);
	m_remotePlayerLabel->setObjectName("RemotePlayerLabel");

	m_rulesButton = new QPushButton("Rules", this);
	m_rulesButton->setObjectName("RulesButton");
	m_optionsButton = new QPushButton("Options", this);
	m_optionsButton->setObjectName("OptionsButton");
	m_forfeitButton = new QPushButton("Forfeit", this);
	m_forfeitButton->setObjectName("ForfeitButton");

	m_bingoBoard = new BingoBoard(boardSize, localPlayer, this);
	m_bingoBoard->setObjectName("BingoBoard");

	m_bingoBoard->PopulateBoard();
}

void BoardWindow::ConnectButtons()
{
	connect(m_bingoBoard, &BingoBoard::SpaceSelected, this,
		&BoardWindow::SelectLocalSpace);
	connect(m_bingoBoard, &BingoBoard::Selectable, this,
		&BoardWindow::SetSelectable);

	connect(m_receiver, &BingoReceiver::Received, this,
		&BoardWindow::SelectRemoteSpace);
}

void BoardWindow::SetSelectable(int id) 
{
	m_bingoBoard->SetSelectable(id);
}

void BoardWindow::SelectLocalSpace(int id)
{
	localPlayer->score = localPlayer->score + 1;
	m_localPlayerLabel->setText(localPlayer->username +
		QString::fromStdString(" x ") + QString::number(localPlayer->score));

	m_sender->Send(id);
}

void BoardWindow::SelectRemoteSpace(int id) 
{
	remotePlayer->score = remotePlayer->score + 1;
	m_remotePlayerLabel->setText(remotePlayer->username +
		QString::fromStdString(" x ") + QString::number(remotePlayer->score));
}

void BoardWindow::DisplayGameView()
{
	//int height = screenGeometry.height() - 40;
	//int width = (int)((double)height * aspect_ratio);
	//int stretch = (int)
	//	((double)((screenGeometry.width() - width) / screenGeometry.width())) * 10;

	//m_menuWidget->setLayout(mainLayout);
	m_menuWidget->setLayout(m_menuLayout);

	m_menuLayout->addWidget(m_rulesButton);
	m_menuLayout->addWidget(m_optionsButton);
	m_menuLayout->addWidget(m_forfeitButton);

	mainLayout->addWidget(m_logoLabel_SuperMario);
	mainLayout->addWidget(m_logoLabel_Sunshine);
	mainLayout->addWidget(m_logoLabel_Bingo);
	mainLayout->addWidget(m_timerLabel);
	mainLayout->addWidget(m_localPlayerLabel);
	mainLayout->addWidget(m_remotePlayerLabel);
	mainLayout->addWidget(m_bingoBoard);
	mainLayout->addWidget(m_menuWidget);

	mainLayout->setAlignment(m_logoLabel_SuperMario, Qt::AlignHCenter);
	mainLayout->setAlignment(m_logoLabel_Sunshine, Qt::AlignHCenter);
	mainLayout->setAlignment(m_logoLabel_Bingo, Qt::AlignHCenter);
	mainLayout->setAlignment(m_timerLabel, Qt::AlignHCenter);
	mainLayout->setAlignment(m_localPlayerLabel, Qt::AlignHCenter);
	mainLayout->setAlignment(m_remotePlayerLabel, Qt::AlignHCenter);
	mainLayout->setAlignment(m_bingoBoard, Qt::AlignHCenter);
	mainLayout->setAlignment(m_menuWidget, Qt::AlignHCenter);

	StylizeText();
	this->setStyleSheet(m_boardSS);
	m_centralWidget->setLayout(mainLayout);
	setCentralWidget(m_centralWidget);

	m_localPlayerLabel->setText(localPlayer->username +
		QString::fromStdString(" x ") + QString::number(localPlayer->score));
	m_remotePlayerLabel->setText(remotePlayer->username +
		QString::fromStdString(" x ") + QString::number(remotePlayer->score));
}

void BoardWindow::StylizeText()
{
	QGraphicsDropShadowEffect *shadowEffect =
		new QGraphicsDropShadowEffect();
	shadowEffect->setBlurRadius(6);
	shadowEffect->setXOffset(1);
	shadowEffect->setYOffset(3);
	shadowEffect->setColor(qRgba(0, 0, 0, 0.16));
	QGraphicsDropShadowEffect *shadowEffect_logo_supermario =
		new QGraphicsDropShadowEffect();
	shadowEffect_logo_supermario->setBlurRadius(6);
	shadowEffect_logo_supermario->setXOffset(1);
	shadowEffect_logo_supermario->setYOffset(3);
	shadowEffect_logo_supermario->setColor(qRgba(0, 0, 0, 0.7));
	QGraphicsDropShadowEffect *shadowEffect_logo_sunshine =
		new QGraphicsDropShadowEffect();
	shadowEffect_logo_sunshine->setBlurRadius(6);
	shadowEffect_logo_sunshine->setXOffset(1);
	shadowEffect_logo_sunshine->setYOffset(3);
	shadowEffect_logo_sunshine->setColor(qRgba(0, 0, 0, 0.7));
	QGraphicsDropShadowEffect *shadowEffect_logo_bingo =
		new QGraphicsDropShadowEffect();
	shadowEffect_logo_bingo->setBlurRadius(6);
	shadowEffect_logo_bingo->setXOffset(1);
	shadowEffect_logo_bingo->setYOffset(3);
	shadowEffect->setColor(qRgba(0, 0, 0, 0.7));
	QGraphicsDropShadowEffect *shadowEffect_timer =
		new QGraphicsDropShadowEffect();
	shadowEffect_timer->setBlurRadius(6);
	shadowEffect_timer->setXOffset(1);
	shadowEffect_timer->setYOffset(3);
	shadowEffect_timer->setColor(qRgba(0, 0, 0, 0.7));
	QGraphicsDropShadowEffect *shadowEffect_localPlayer =
		new QGraphicsDropShadowEffect();
	shadowEffect_localPlayer->setBlurRadius(6);
	shadowEffect_localPlayer->setXOffset(1);
	shadowEffect_localPlayer->setYOffset(3);
	shadowEffect_localPlayer->setColor(qRgba(0, 0, 0, 0.5));
	QGraphicsDropShadowEffect *shadowEffect_remotePlayer =
		new QGraphicsDropShadowEffect();
	shadowEffect_remotePlayer->setBlurRadius(6);
	shadowEffect_remotePlayer->setXOffset(1);
	shadowEffect_remotePlayer->setYOffset(3);
	shadowEffect_remotePlayer->setColor(qRgba(0, 0, 0, 0.5));

	m_logoLabel_SuperMario->setGraphicsEffect(shadowEffect_logo_supermario);
	m_logoLabel_Sunshine->setGraphicsEffect(shadowEffect_logo_sunshine);
	m_logoLabel_Bingo->setGraphicsEffect(shadowEffect_logo_bingo);
	m_timerLabel->setGraphicsEffect(shadowEffect_timer);
	m_localPlayerLabel->setGraphicsEffect(shadowEffect_localPlayer);
	m_remotePlayerLabel->setGraphicsEffect(shadowEffect_remotePlayer);
}