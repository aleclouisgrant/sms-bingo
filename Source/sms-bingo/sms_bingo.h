#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_sms_bingo.h"

class sms_bingo : public QMainWindow
{
	Q_OBJECT

public:
	sms_bingo(QWidget *parent = Q_NULLPTR);

private:
	Ui::sms_bingoClass ui;
};
