#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include "BingoButton.h"

class HomeWindow : public QMainWindow
{
	Q_OBJECT

public:
	HomeWindow(QWidget *parent = Q_NULLPTR);
	~HomeWindow();

private:
	QWidget* m_centralWidget;
	QWidget* m_optionsWidget;

	QVBoxLayout* m_centralLayout;
	QVBoxLayout* m_optionsLayout;
	
	QLabel* m_optionsLabel;
	QLabel* m_lockoutLabel;
	QLabel* m_connect5Label;
	QLabel* m_allowDelayClickLabel;
	BingoButton* m_bingoButton;

};
