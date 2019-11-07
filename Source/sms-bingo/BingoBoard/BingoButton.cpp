#include "BingoButton.h"
#include <QHBoxLayout>
#include <QFile>

BingoButton::BingoButton(QWidget *parent)
	: QPushButton(parent)
{
	m_centralWidget = new QWidget(parent);

	m_label = new QLabel("Default Text", m_centralWidget);
	auto layout = new QHBoxLayout(m_centralWidget);
	
	m_label->setWordWrap(true);
	layout->addWidget(m_label, 0, Qt::AlignCenter);
	
	setLayout(layout);

	//QPushButton::setEnabled(false);
	m_selectable = false;
	m_selected = false;
}

BingoButton::~BingoButton()
{
}

/*
Sets the text of the QLabel inside the BingoButton widget
*/
void BingoButton::setText(QString text)
{
	m_label->setText(text);
}

void BingoButton::setMinimumSize(int dim)
{
	m_centralWidget->setMinimumSize(dim, dim);
	QPushButton::setMinimumSize(dim, dim);
	//m_label->setMinimumSize(dim, m_label->minimumHeight());
}

void BingoButton::setMaximumSize(int dim)
{
	m_centralWidget->setMaximumSize(dim, dim);
	QPushButton::setMaximumSize(dim, dim);
	//m_label->setMaximumSize(dim, m_label->maximumHeight());
}

/*
Assigns a BingoSpace to the BingoButton 
*/
void BingoButton::setBingoSpace(BingoSpace *bingoSpace) {
	m_bingoSpace = bingoSpace;
	std::string title = bingoSpace->GetText();
	setText(QString::fromStdString(title));
}

bool BingoButton::isSelectable() {
	return m_selectable;
}

bool BingoButton::isSelected() {
	return m_selected;
}

/*
Checks whether the conditions of the BingoButton's BingoSpace to be
selectable to the Player have been met and emits the Selectable signal
to be picked up by the BoardWindow if so.
*/
bool BingoButton::CheckSpace() {
	if (m_bingoSpace->CheckConditions()) {
		m_selectable = true;
		QPushButton::setEnabled(true);
		return true;
	}
	return false;
}


void BingoButton::SelectSpace() {
	QPushButton::setEnabled(false);
	m_selectable = false;
	m_selected = true;
}