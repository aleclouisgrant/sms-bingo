#pragma once

#include <QWidget>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QColor>
#include "BingoSpace.h"

class BingoButton : public QPushButton
{
	Q_OBJECT

public:
	BingoButton(QWidget *parent);
	~BingoButton();

	void setText(QString text);
	void setMinimumSize(int dim);
	void setMaximumSize(int dim);
	void setBingoSpace(BingoSpace *bingoSpace);

	bool isSelectable();
	bool isSelected();

	bool CheckSpace();
	void SelectSpace();

private:
	QWidget *m_centralWidget;
	QLabel *m_label;

	bool m_selectable;
	bool m_selected;
	BingoSpace *m_bingoSpace;
};
