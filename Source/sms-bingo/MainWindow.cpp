#include "MainWindow.h"
#include <qguiapplication.h>
#include <QGraphicsEffect>
#include <QSizePolicy>
#include <QFile>
#include <QResizeEvent>


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	boardSize = 5;
	aspect_ratio = 1.2121212;

	screen = QGuiApplication::primaryScreen();
	screenGeometry = screen->geometry();

	ReadStyleSheets();
	MakePlayers();
	MakeWidgets();
	DisplayGameView();
	ConnectButtons();
}


MainWindow::~MainWindow()
{

}

void MainWindow::ReadStyleSheets() 
{
	QFile file(":/QSS/gameView.qss");
	file.open(QFile::ReadOnly);
	gameViewSS = QLatin1String(file.readAll());
	file.close();

	file.setFileName(":/QSS/boardView.qss");
	file.open(QFile::ReadOnly);
	boardViewSS = QLatin1String(file.readAll());
	file.close();

	file.setFileName(":/QSS/selectspace.qss");
	file.open(QFile::ReadOnly);
	selectSpaceSS = QLatin1String(file.readAll());
	file.close();
}

void MainWindow::MakePlayers() 
{
	localPlayer = new Player("biggusdickus");
	remotePlayer = new Player("asianricekitteh");

}

void MainWindow::MakeWidgets()
{	
	m_centralWidget = new QWidget;
	m_centralWidget->setObjectName("CentralWidget");
	m_rightWidget = new QWidget;
	m_rightWidget->setObjectName("RightWidget");
	m_bingoWidget = new QWidget;
	m_bingoWidget->setObjectName("BingoWidget");

	m_hLayout = new QHBoxLayout(this);
	m_rightLayout = new QVBoxLayout(this);
	m_bingoGrid = new QGridLayout(this);

	m_frame = new QFrame(this);
	m_frame->setObjectName("GameFrame");

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

	m_button = new QPushButton("Toggle View", this);
	m_button->setObjectName("MenuButton");

	MakeBingoBoard();
}

void MainWindow::MakeBingoBoard()
{
	m_bingoButtonGroup = new QButtonGroup(this);

	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			int index = (i*boardSize) + j;
			QString btnText = QString::fromStdString("Option " 
				+ std::to_string(index));
			QPushButton *btn = new QPushButton(btnText, this);

			m_bingoButtonGroup->addButton(btn);
			m_bingoButtonGroup->setId(btn, (i*boardSize) + j);
			m_bingoGrid->addWidget(btn, i, j);
		}
	}
}


void MainWindow::ConnectButtons() 
{
	connect(m_button, &QPushButton::released, this,
		&MainWindow::ToggleView);
	connect(m_bingoButtonGroup,
		QOverload<int>::of(&QButtonGroup::buttonClicked), this,
		&MainWindow::SelectSpace);
}

void MainWindow::SelectSpace(int id) 
{	
	m_bingoButtonGroup->button(id)->setStyleSheet(selectSpaceSS);
	localPlayer->score = localPlayer->score + 1;
	m_localPlayerLabel->setText(localPlayer->username +
		QString::fromStdString(" x ") + QString::number(localPlayer->score));
}


void MainWindow::ToggleView()
{
	switch (m_view) {
		case View::GAMEVIEW:
			DisplayBoardView();
			break;
		case View::BOARDVIEW:
			DisplayGameView();
			break;
	}
}

void MainWindow::DisplayGameView()
{
	m_view = GAMEVIEW;

	RemoveWidgets();

	int height = screenGeometry.height() - 40;
	int width = (int)((double)height * aspect_ratio);

	int stretch = (int)
		((double)((screenGeometry.width() - width) / screenGeometry.width())) * 10;

	m_frame->setMinimumSize(width, height);
	m_frame->setMaximumSize(width, height);

	m_hLayout->addWidget(m_frame, stretch);
	m_hLayout->addWidget(m_rightWidget, 10 - stretch);
	m_rightWidget->setLayout(m_rightLayout);

	m_bingoWidget->setLayout(m_bingoGrid);

	m_rightLayout->addWidget(m_logoLabel_SuperMario);
	m_rightLayout->addWidget(m_logoLabel_Sunshine);
	m_rightLayout->addWidget(m_logoLabel_Bingo);
	m_rightLayout->addWidget(m_timerLabel);
	m_rightLayout->addWidget(m_localPlayerLabel);
	m_rightLayout->addWidget(m_remotePlayerLabel);
	m_rightLayout->addWidget(m_bingoWidget);
	m_rightLayout->addWidget(m_button);

	m_rightLayout->setAlignment(m_logoLabel_SuperMario, Qt::AlignHCenter);
	m_rightLayout->setAlignment(m_logoLabel_Sunshine, Qt::AlignHCenter);
	m_rightLayout->setAlignment(m_logoLabel_Bingo, Qt::AlignHCenter);
	m_rightLayout->setAlignment(m_timerLabel, Qt::AlignHCenter);
	m_rightLayout->setAlignment(m_localPlayerLabel, Qt::AlignHCenter);
	m_rightLayout->setAlignment(m_remotePlayerLabel, Qt::AlignHCenter);
	m_rightLayout->setAlignment(m_bingoGrid, Qt::AlignHCenter);
	m_rightLayout->setAlignment(m_button, Qt::AlignHCenter);

	StylizeText();
	this->setStyleSheet(gameViewSS);
	m_centralWidget->setLayout(m_hLayout);
	setCentralWidget(m_centralWidget);

	int boardIndex = boardSize * boardSize;
	width = m_bingoButtonGroup->button(0)->width();
	
	for (int i = 0; i < boardIndex; i++) {
		m_bingoButtonGroup->button(i)->setMinimumSize(width, width);
	}

	m_localPlayerLabel->setText(localPlayer->username +
		QString::fromStdString(" x ") + QString::number(m_bingoButtonGroup->button(0)->height()));
	m_remotePlayerLabel->setText(remotePlayer->username +
		QString::fromStdString(" x ") + QString::number(remotePlayer->score));

}

void MainWindow::DisplayBoardView()
{
	m_view = BOARDVIEW;

	RemoveWidgets();

	int height = (int)((double)(screenGeometry.height() * 0.6)) - 40;
	int width = (int)((double)height * aspect_ratio);

	m_frame->setMinimumSize(width, height);
	m_frame->setMaximumSize(width, height);

	int stretch = (int)
		((double)((screenGeometry.width() - width) / screenGeometry.width())) * 10;

	m_hLayout->addWidget(m_bingoWidget, 10 - stretch);
	m_hLayout->addWidget(m_rightWidget, stretch);

	m_bingoWidget->setLayout(m_bingoGrid);
	m_rightWidget->setLayout(m_rightLayout);

	m_rightLayout->addWidget(m_logoLabel_SuperMario);
	m_rightLayout->addWidget(m_logoLabel_Sunshine);
	m_rightLayout->addWidget(m_logoLabel_Bingo);
	m_rightLayout->addWidget(m_localPlayerLabel);
	m_rightLayout->addWidget(m_remotePlayerLabel);
	m_rightLayout->addWidget(m_frame);
	m_rightLayout->addWidget(m_button);

	m_rightLayout->setAlignment(m_logoLabel_SuperMario, Qt::AlignHCenter);
	m_rightLayout->setAlignment(m_logoLabel_Sunshine, Qt::AlignHCenter);
	m_rightLayout->setAlignment(m_logoLabel_Bingo, Qt::AlignHCenter);
	m_rightLayout->setAlignment(m_localPlayerLabel, Qt::AlignHCenter);
	m_rightLayout->setAlignment(m_remotePlayerLabel, Qt::AlignHCenter);
	m_rightLayout->setAlignment(m_frame, Qt::AlignHCenter);
	m_rightLayout->setAlignment(m_button, Qt::AlignHCenter);

	this->setStyleSheet(boardViewSS);
	m_centralWidget->setLayout(m_hLayout);
	setCentralWidget(m_centralWidget);

	int boardIndex = boardSize * boardSize;
	width = m_bingoButtonGroup->button(0)->width();

	for (int i = 0; i < boardIndex; i++) {
		m_bingoButtonGroup->button(i)->resize(width, width);
	}

	m_localPlayerLabel->setText(localPlayer->username +
		QString::fromStdString(" (") + QString::number(localPlayer->elo) + 
		QString::fromStdString(")"));
	m_remotePlayerLabel->setText(remotePlayer->username +
		QString::fromStdString(" (") + QString::number(remotePlayer->elo) +
		QString::fromStdString(")"));
}

void MainWindow::RemoveWidgets()
{
	switch (m_view) {
	case View::GAMEVIEW:
		m_rightLayout->removeWidget(m_logoLabel_SuperMario);
		m_rightLayout->removeWidget(m_logoLabel_Sunshine);
		m_rightLayout->removeWidget(m_logoLabel_Bingo);
		m_rightLayout->removeWidget(m_timerLabel);
		m_rightLayout->removeWidget(m_localPlayerLabel);
		m_rightLayout->removeWidget(m_remotePlayerLabel);
		m_rightLayout->removeWidget(m_frame);
		m_rightLayout->removeWidget(m_button);

		m_hLayout->removeWidget(m_rightWidget);
		m_hLayout->removeWidget(m_bingoWidget);
		break;
	case View::BOARDVIEW:
		m_rightLayout->removeWidget(m_logoLabel_SuperMario);
		m_rightLayout->removeWidget(m_logoLabel_Sunshine);
		m_rightLayout->removeWidget(m_logoLabel_Bingo);
		m_rightLayout->removeWidget(m_timerLabel);
		m_rightLayout->removeWidget(m_localPlayerLabel);
		m_rightLayout->removeWidget(m_remotePlayerLabel);
		m_rightLayout->removeWidget(m_bingoWidget);
		m_rightLayout->removeWidget(m_button);

		m_hLayout->removeWidget(m_rightWidget);
		m_hLayout->removeWidget(m_frame);
		break;
	}
}

void MainWindow::StylizeText()
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
	m_bingoButtonGroup->button(1)->setGraphicsEffect(shadowEffect);
}