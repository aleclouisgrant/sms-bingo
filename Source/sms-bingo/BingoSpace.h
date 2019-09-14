#pragma once
#include <QtWidgets/QMainWindow>
#include <QPushButton>

class BingoSpace : public QPushButton
{
	Q_OBJECT

public:
	BingoSpace();
	~BingoSpace();

	int boardsize;

protected:
	virtual void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;

};