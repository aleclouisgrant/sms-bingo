#include "HomeWindow.h"

HomeWindow::HomeWindow(QWidget *parent)
	: QMainWindow(parent)
{
	m_centralWidget = new QWidget;
	m_centralWidget->setObjectName("CentralWidget");
	m_optionsWidget = new QWidget;
	m_optionsWidget->setObjectName("OptionsWidget");

	m_centralLayout = new QVBoxLayout(this);
	m_optionsLayout = new QVBoxLayout(this);

	m_optionsLabel = new QLabel("Options", this);
	m_optionsLabel->setObjectName("OptionsLabel");
	m_lockoutLabel = new QLabel("Lockout:", this);
	m_lockoutLabel->setObjectName("LockoutLabel");
	m_connect5Label = new QLabel("Connect 5:", this);
	m_connect5Label->setObjectName("Connect5Label");
	m_allowDelayClickLabel = new QLabel("Allow delayed click:", this);
	m_allowDelayClickLabel->setObjectName("AllowDelayClickLabel");
	
	m_bingoButton = new BingoButton(this);
	m_bingoButton->setMinimumSize(100);
	m_bingoButton->setMaximumSize(100);
	m_bingoButton->setText("HELLO LETS SEE IF THIS BAD BOY WORD WRAPS OR NOT");

	m_optionsWidget->setLayout(m_optionsLayout);
	m_optionsLayout->addWidget(m_optionsLabel);
	m_optionsLayout->addWidget(m_lockoutLabel);
	m_optionsLayout->addWidget(m_connect5Label);
	m_optionsLayout->addWidget(m_allowDelayClickLabel);
	m_optionsLayout->addWidget(m_bingoButton);

	m_centralWidget->setLayout(m_centralLayout);
	m_centralLayout->addWidget(m_optionsWidget);
	setCentralWidget(m_centralWidget);
}

HomeWindow::~HomeWindow()
{
}
