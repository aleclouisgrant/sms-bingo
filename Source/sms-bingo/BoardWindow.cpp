#include "BoardWindow.h"
#include <qguiapplication.h>
#include <QGraphicsEffect>
#include <QSizePolicy>
#include <QFile>
#include <QResizeEvent>
#include "BingoButton.h"
#include <DolphinProcess/DolphinAccessor.h>
#include <MemoryScanner/MemoryScanner.h>

BoardWindow::BoardWindow(QWidget *parent)
	: QMainWindow(parent)
{
	boardSize = 5;
	aspect_ratio = 1.2121212;

	screen = QGuiApplication::primaryScreen();
	screenGeometry = screen->geometry();

	HookToDolphin();
	ReadStyleSheets();
	MakePlayers();
	MakeWidgets();
	DisplayGameView();
	PopulateBoard();
	ConnectButtons();
}


BoardWindow::~BoardWindow()
{

}

void BoardWindow::HookToDolphin() 
{
	DolphinComm::DolphinAccessor::hook();
	if (DolphinComm::DolphinAccessor::getStatus() ==
		DolphinComm::DolphinAccessor::DolphinStatus::hooked) {
		m_memscanner = new MemoryScanner();
		m_memscanner->Start();
	}
}

void BoardWindow::ReadStyleSheets()
{
	QFile file(":/QSS/board.qss");
	file.open(QFile::ReadOnly);
	boardSS = QLatin1String(file.readAll());
	file.close();
}

void BoardWindow::MakePlayers()
{
	localPlayer = new Player("biggusdickus");
	remotePlayer = new Player("ricekitteh");
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
	m_bingoGrid = new QGridLayout(this);
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

	MakeBingoBoard();
}

void BoardWindow::MakeBingoBoard()
{
	m_bingoButtonGroup = new BingoButtonGroup(this);

	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			int index = (i*boardSize) + j;
			QString btnText = QString::fromStdString("Option "
				+ std::to_string(index));
			BingoButton *btn = new BingoButton(this);
			btn->setText(btnText);
			btn->setMinimumSize(110);
			btn->setMaximumSize(110);

			m_bingoButtonGroup->addButton(btn);
			m_bingoButtonGroup->setId(btn, (i*boardSize) + j);
			m_bingoGrid->addWidget(btn, i, j);
		}
	}
}

void BoardWindow::ConnectButtons()
{
	connect(m_rulesButton, &QPushButton::released, this,
		&BoardWindow::ToggleView);
	connect(m_bingoButtonGroup,
		QOverload<int>::of(&QButtonGroup::buttonClicked), this,
		&BoardWindow::SelectSpace);

	connect(m_memscanner, &MemoryScanner::BingoSpace_OneShine, this,
		&BoardWindow::SelectSpace);
	connect(m_memscanner, &MemoryScanner::ValueChanged, this,
		&BoardWindow::SelectSpace);
}

void BoardWindow::SelectSpace(int id)
{
	m_bingoButtonGroup->selectSpace(id);
	localPlayer->score = localPlayer->score + 1;
	m_localPlayerLabel->setText(localPlayer->username +
		QString::fromStdString(" x ") + QString::number(localPlayer->score));
}


void BoardWindow::ToggleView()
{

}

void BoardWindow::DisplayGameView()
{
	//int height = screenGeometry.height() - 40;
	//int width = (int)((double)height * aspect_ratio);
	//int stretch = (int)
	//	((double)((screenGeometry.width() - width) / screenGeometry.width())) * 10;

	m_menuWidget->setLayout(mainLayout);
	m_bingoWidget->setLayout(m_bingoGrid);
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
	mainLayout->addWidget(m_bingoWidget);
	mainLayout->addWidget(m_menuWidget);

	mainLayout->setAlignment(m_logoLabel_SuperMario, Qt::AlignHCenter);
	mainLayout->setAlignment(m_logoLabel_Sunshine, Qt::AlignHCenter);
	mainLayout->setAlignment(m_logoLabel_Bingo, Qt::AlignHCenter);
	mainLayout->setAlignment(m_timerLabel, Qt::AlignHCenter);
	mainLayout->setAlignment(m_localPlayerLabel, Qt::AlignHCenter);
	mainLayout->setAlignment(m_remotePlayerLabel, Qt::AlignHCenter);
	mainLayout->setAlignment(m_bingoGrid, Qt::AlignHCenter);

	StylizeText();
	this->setStyleSheet(boardSS);
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

void BoardWindow::PopulateBoard() {
	QString spaceArray[] = {
		//"Collect 6 Secret Shines",
		"Collect 17 Blue Coins",
		"Collect 1 Hidden Shine in Ricco Harbor",
		"15 Blue Coins from M Graffiti",
		"55 Total Blue Coins",
		"Pinna Park Episode 8 Shine",
		"Ride Yoshi in Pinna Park",
		"Collect 2 Hidden Shines in Pianta Village",
		"20 Blue Coins from Noki Bay",
		"100 Coin Shine from Ricco Harbor",
		//"100 Coin Shine from Gelato Beach",
		"Collect 4 Shines",
		"30 Total Blue Coins",
		"Pinna 3 Hoverless",
		"Bianco Hills Episode 8 Shine",
		"11 Shines from Delfino Plaza",
		"Defeat Shadow Mario in Gelato Beach",
		"Defeat the Eel of Noki Bay",
		"Lily Pad Shine",
		"5 Linked Blue Coin Pairs",
		"20 Blue Coins from Pinna Park",
		"10 Blue Coins from Noki Bay",
		"Collect 1 Hidden Shine from Noki Bay",
		"25 Lives",
		"Beat Piantissimo in Noki Bay",
		"2 Shadow Marios",
		"3 Fruit Lady Blue Coins"
	};

	for (int i = 0; i < boardSize*boardSize; i++) {
		m_bingoButtonGroup->setText(i, spaceArray[i]);
	}
}