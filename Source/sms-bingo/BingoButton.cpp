#include "BingoButton.h"
#include <QHBoxLayout>

BingoButton::BingoButton(QWidget *parent)
	: QPushButton(parent)
{
	m_centralWidget = new QWidget(parent);

	m_label = new QLabel("Default Text", m_centralWidget);
	auto layout = new QHBoxLayout(m_centralWidget);
	
	m_label->setWordWrap(true);
	layout->addWidget(m_label, 0, Qt::AlignCenter);
	
	setLayout(layout);
}

BingoButton::~BingoButton()
{
}


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
