#pragma once

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QMediaPlaylist>
#include <QGridLayout>


class TitleWindow : public QMainWindow
{
	Q_OBJECT

public:
	TitleWindow(QWidget *parent = Q_NULLPTR);
	~TitleWindow();

private:
	QString titleSS;

	QMediaPlayer *m_mediaPlayer;
	QMediaPlaylist *m_playlist;

	QLabel *m_loginLabel;
	QLabel *m_usernameLabel;
	QLabel *m_passwordLabel;
	QLabel *m_continueLabel;

	QWidget *m_centralWidget;
	QVideoWidget *m_videoWidget;
	QWidget *m_middleWidget;

	QGridLayout *m_layout;
	QVBoxLayout *m_middleLayout;
};
