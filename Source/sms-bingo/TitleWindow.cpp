#include "TitleWindow.h"
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QLabel>
#include <QFile>

TitleWindow::TitleWindow(QWidget *parent)
	: QMainWindow(parent)
{
	QFile file(":/QSS/title.qss");
	file.open(QFile::ReadOnly);
	titleSS = QLatin1String(file.readAll());
	file.close();
	this->setStyleSheet(titleSS);

	m_centralWidget = new QWidget(this);
	m_middleWidget = new QWidget(m_centralWidget);
	m_middleWidget->setObjectName("LoginWidget");
	m_videoWidget = new QVideoWidget(m_centralWidget);

	m_layout = new QGridLayout;
	m_middleLayout = new QVBoxLayout;

	m_mediaPlayer = new QMediaPlayer;
	m_playlist = new QMediaPlaylist;

	m_loginLabel = new QLabel("Log in:", m_middleWidget);
	m_usernameLabel = new QLabel("Username", m_middleWidget);
	m_passwordLabel = new QLabel("Password:", m_middleWidget);
	m_continueLabel = new QLabel("Enter to continue", m_middleWidget);

	m_middleWidget->setLayout(m_middleLayout);
	m_middleLayout->addWidget(m_loginLabel);
	m_middleLayout->addWidget(m_usernameLabel);
	m_middleLayout->addWidget(m_passwordLabel);
	m_middleLayout->addWidget(m_continueLabel);

	m_centralWidget->setLayout(m_layout);
	m_layout->addWidget(m_videoWidget);
	m_layout->addWidget(m_middleWidget, 0, 0, Qt::AlignCenter|Qt::AlignHCenter);
	m_middleWidget->raise();

	m_mediaPlayer->setVideoOutput(m_videoWidget);
	m_playlist->addMedia(QUrl::fromLocalFile("E:/SMS Bingo/sms-bingo/Source/sms-bingo/Resources/titlevideo.wmv"));
	m_playlist->setCurrentIndex(1);
	m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
	m_mediaPlayer->setPlaylist(m_playlist);

	layout()->setContentsMargins(0, 0, 0, 0);
	m_layout->getContentsMargins(0, 0, 0, 0);
	m_middleLayout->getContentsMargins(0, 0, 0, 0);
	layout()->setSpacing(0);
	m_layout->setSpacing(0);
	m_middleLayout->setSpacing(0);

	setCentralWidget(m_centralWidget);
	m_mediaPlayer->play();
}


TitleWindow::~TitleWindow()
{
}
