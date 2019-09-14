#pragma once

#include <QWidget>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QColor>

class BingoButton : public QPushButton
{
	Q_OBJECT

public:
	BingoButton(QWidget *parent);
	~BingoButton();

	void setText(QString text);
	void setMinimumSize(int dim);
	void setMaximumSize(int dim);

private:
	QWidget *m_centralWidget;
	QLabel *m_label;
};
