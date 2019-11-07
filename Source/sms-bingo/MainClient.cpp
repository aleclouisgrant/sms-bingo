#include "MainClient.h"
#include <WS2tcpip.h>
#include <qdebug.h>

#include <thread>

#include <Windows.h>

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "RoomWidget.h"
#include "ServerClient/BingoReceiver.h"
#include "ServerClient/BingoSender.h"
#include "ServerClient/BingoServer.h"
#include "ServerClient/BingoClient.h"
#include "QWindows/BoardWindow.h"
#include "MainServer.h"

#pragma comment (lib, "ws2_32.lib")

#define IP_ADDR "127.0.0.1"
#define PORT_NUMBER 54000
#define BUFFER_SIZE 4096


MainClient::MainClient(QWidget *parent)
	: QMainWindow(parent)
{
	m_mainWidget = new QWidget(this);
	LoginGUI();
	
	std::thread t(&MainClient::ConnectToServer, this);
	t.detach(); //probably no need to detach
}

MainClient::~MainClient()
{
}


void MainClient::LoginGUI()
{
	m_layout = new QVBoxLayout(m_mainWidget);

	m_logoWidget = new QWidget(m_mainWidget);
	QVBoxLayout *logoLayout = new QVBoxLayout(m_logoWidget);
	QLabel *logoLabel = new QLabel("Super Mario Sunshine Bingo", m_logoWidget);
	QLabel *versionLabel = new QLabel("VER 1.0", m_logoWidget);

	logoLayout->addWidget(logoLabel);
	logoLayout->addWidget(versionLabel);
	m_logoWidget->setLayout(logoLayout);

	m_loginWidget = new QFrame(m_mainWidget);
	QVBoxLayout *loginLayout = new QVBoxLayout(m_loginWidget);
	QLabel *loginLabel = new QLabel("Select a username:", m_loginWidget);
	m_userInput = new QLineEdit("Username", m_loginWidget);

	loginLayout->addWidget(loginLabel);
	loginLayout->addWidget(m_userInput);
	m_loginWidget->setLayout(loginLayout);

	m_buttonsWidget = new QWidget(m_mainWidget);
	QHBoxLayout *buttonLayout = new QHBoxLayout(m_buttonsWidget);
	QPushButton *exitButton = new QPushButton("Exit", m_buttonsWidget);
	QPushButton *aboutButton = new QPushButton("About", m_buttonsWidget);
	QPushButton *continueButton = new QPushButton("Continue", m_buttonsWidget);

	buttonLayout->addWidget(exitButton);
	buttonLayout->addWidget(aboutButton);
	buttonLayout->addWidget(continueButton);
	m_buttonsWidget->setLayout(buttonLayout);

	connect(exitButton, &QPushButton::released, this, &MainClient::MakeHomeWindow);
	connect(aboutButton, &QPushButton::released, this, &MainClient::DisplayAbout);
	connect(continueButton, &QPushButton::released, this, &MainClient::CheckUsername);

	m_layout->addWidget(m_logoWidget);
	m_layout->addWidget(m_loginWidget);
	m_layout->addWidget(m_buttonsWidget);
	m_mainWidget->setLayout(m_layout);

	setCentralWidget(m_mainWidget);

	m_loginWidget->setStyleSheet(
		"QFrame{background: rgba(0,25,255,0.6); border-radius: 10px;}"
	);


	m_loginWidget->setMinimumSize(450, 250);
	m_loginWidget->setMaximumSize(450, 250);
	m_layout->setAlignment(m_loginWidget, Qt::AlignHCenter);
	m_layout->setAlignment(m_logoWidget, Qt::AlignHCenter);
	m_layout->setAlignment(m_buttonsWidget, Qt::AlignBottom | Qt::AlignHCenter);

	loginLayout->setAlignment(loginLabel, Qt::AlignHCenter);
	loginLayout->setAlignment(m_userInput, Qt::AlignHCenter);

}

void MainClient::DisplayAbout()
{
	m_loginWidget->hide();
	m_buttonsWidget->hide();

	QString about = "Super Mario Sunshine Bingo ver 1.0 developer: aleclouisgrant";

	QWidget *aboutWidget = new QWidget(m_mainWidget);
	QVBoxLayout *aboutLayout = new QVBoxLayout(aboutWidget);
	QLabel *aboutText = new QLabel(about, aboutWidget);
	aboutText->setWordWrap(true);
	QPushButton *backButton = new QPushButton("Back", aboutWidget);
	
	aboutLayout->addWidget(aboutText);
	aboutLayout->addWidget(backButton);
	aboutWidget->setLayout(aboutLayout);

	m_layout->addWidget(aboutWidget);

	connect(backButton, &QPushButton::released, this, &MainClient::MakeHomeWindow);
}

void MainClient::CheckUsername()
{
	// send a flag to the server to check the username
	QString username = m_userInput->text();

	if (username.length() < 1) { //username too short
		return;
	}

	qDebug() << "CLIENT(" << username << "): Request to check username";
	char buf[BUFFER_SIZE];
	memset(buf, 0, BUFFER_SIZE);
	sprintf(buf, "%d", ServerFlag::CheckUsername);
	send(m_serverSocket, buf, BUFFER_SIZE, 0);
	
	memset(buf, 0, BUFFER_SIZE);
	sprintf(buf, "%s", username.toLocal8Bit().data());
	qDebug() << "CLIENT(" << username << "): Checking username" << buf;
	send(m_serverSocket, buf, BUFFER_SIZE, 0);
	memset(buf, 0, BUFFER_SIZE);

	ServerFlag flag;
	recv(m_serverSocket, buf, BUFFER_SIZE, 0);	
	sscanf(buf, "%d", &flag);

	if (flag == ServerFlag::UsernameAccept) { //username passed checks
		m_username = username;
		qDebug() << "CLIENT (" << m_username << "): Username accepted by server.";
		std::thread t(&MainClient::Listen, this);
		t.detach();
		RoomGUI();
	}
	else if (flag == ServerFlag::UsernameReject) { //username is already taken
		qDebug() << "CLIENT(" << username << "): rejected by server.";
		//say something here
		return;
	}
}

void MainClient::RoomGUI()
{
	//refresh the room list
	char buf[BUFFER_SIZE];
	sprintf(buf, "%d", ServerFlag::RefreshRooms);
	send(m_serverSocket, buf, BUFFER_SIZE, 0);

	connect(this, &MainClient::Refresh, this, &MainClient::RefreshRoomList);
	connect(this, &MainClient::IncomingRequest, this, &MainClient::ShowRequest);

	// we can now delete all the widgets from before
	qDeleteAll(m_mainWidget->children());
	m_layout = new QVBoxLayout(m_mainWidget);

	QLabel *usernameLabel = new QLabel(m_username, m_mainWidget);

	m_roomWidget = new QFrame(m_mainWidget);
	m_roomLayout = new QVBoxLayout(m_roomWidget);
	QLabel *joinRoomLabel = new QLabel("JOIN A ROOM", m_roomWidget);

	m_roomLayout->addWidget(joinRoomLabel);
	m_roomWidget->setLayout(m_roomLayout);

	m_buttonsWidget = new QWidget(m_mainWidget);
	QHBoxLayout *buttonLayout = new QHBoxLayout(m_buttonsWidget);
	QPushButton *createRoomButton = new QPushButton("Create Room", m_buttonsWidget);
	QPushButton *optionsButton = new QPushButton("Options", m_buttonsWidget);
	QPushButton *logoutButton = new QPushButton("Logout", m_buttonsWidget);

	buttonLayout->addWidget(createRoomButton);
	buttonLayout->addWidget(optionsButton);
	buttonLayout->addWidget(logoutButton);
	m_buttonsWidget->setLayout(buttonLayout);

	connect(createRoomButton, &QPushButton::released, [this, createRoomButton]() {
		createRoomButton->setEnabled(false);
		CreateRoom();
	} );
	connect(optionsButton, &QPushButton::released, this, &MainClient::DisplayAbout);
	connect(logoutButton, &QPushButton::released, this, &MainClient::LogOut);

	m_layout->addWidget(usernameLabel);
	m_layout->addWidget(m_roomWidget);
	m_layout->addWidget(m_buttonsWidget);
	m_mainWidget->setLayout(m_layout);

	m_roomWidget->setObjectName("RoomWidget");

	m_roomWidget->setStyleSheet(
		"QFrame#RoomWidget{background: rgba(0,25,255,0.6); border-radius: 10px;}"
	);

	joinRoomLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
	m_roomWidget->setMinimumSize(490, 700);
	m_roomWidget->setMaximumSize(490, 700);

	m_roomLayout->setAlignment(Qt::AlignTop);
	m_layout->setAlignment(m_roomWidget, Qt::AlignHCenter);
}

void MainClient::LogOut()
{
	m_username = "";
	qDeleteAll(m_mainWidget->children());
	LoginGUI();
}

void MainClient::CreateRoom()
{
	RoomWidget *roomWidget = new RoomWidget(0, m_username, m_roomWidget);
	m_roomLayout->addWidget(roomWidget);
	m_roomLayout->setAlignment(roomWidget, Qt::AlignTop);

	m_roomArray.append(roomWidget);

	//need to send a message to the mainserver to broadcast the new room to everyone else
	char buf[BUFFER_SIZE];
	memset(buf, 0, BUFFER_SIZE);

	qDebug() << "CLIENT(" << m_username << "): I made a room!";
	sprintf(buf, "%ld", ServerFlag::AddRoom);
	send(m_serverSocket, buf, BUFFER_SIZE, 0);
}

void MainClient::MakeHomeWindow() 
{
	return;
}

void MainClient::ShowRequest(char* username)
{
	qDebug() << "we made it";
	QFrame *requestFrame = new QFrame(this);
	QVBoxLayout *layout = new QVBoxLayout(requestFrame);

	QLabel *text = new QLabel(QString::fromStdString(username) + 
		" would like to play. Accept?", requestFrame);
	
	QWidget *buttonWidget = new QWidget(requestFrame);
	QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);
	QPushButton *acceptButton = new QPushButton("Accept", buttonWidget);
	QPushButton *rejectButton = new QPushButton("Reject", buttonWidget);

	buttonLayout->addWidget(acceptButton);
	buttonLayout->addWidget(rejectButton);

	layout->addWidget(text, Qt::AlignTop | Qt::AlignHCenter);
	layout->addWidget(buttonWidget, Qt::AlignBottom | Qt::AlignHCenter);

	m_layout->addWidget(requestFrame);

	connect(acceptButton, &QPushButton::released, this, &MainClient::ConnectToLocalRoom);

	connect(rejectButton, &QPushButton::released, [requestFrame, this, username] () {
		requestFrame->deleteLater();
		RejectRequest(username);
	});

}

void MainClient::RejectRequest(char *username)
{
	char buf[BUFFER_SIZE];
	memset(buf, 0, BUFFER_SIZE);
	sprintf(buf, "%d", ServerFlag::RequestReject);
	send(m_serverSocket, buf, BUFFER_SIZE, 0);

	memset(buf, 0, BUFFER_SIZE);
	sprintf(buf, "%s", username);
	send(m_serverSocket, buf, BUFFER_SIZE, 0);
}

/*
 * Connect to player as a host
 */
void MainClient::ConnectToLocalRoom() 
{
	char buf[BUFFER_SIZE];
	memset(buf, 0, BUFFER_SIZE);
	sprintf(buf, "%d", ServerFlag::RequestAccept);
	send(m_serverSocket, buf, BUFFER_SIZE, 0);

	BingoReceiver *receiver = new BingoReceiver(0);
	BingoServer *host = new BingoServer(receiver);
	BingoSender *sender = new BingoSender(host);

	//OpenGame();

	BoardWindow *bw = new BoardWindow(sender, receiver);
	bw->show();

	/*
	connect(bw, &QMainWindow::closeEvent, [this] () { 
		this->show();
	}); //listen for game to end to unhide this window

	this->hide();
	*/
}

/*
 * Connect to player as a client
 */
void MainClient::ConnectToRemoteRoom(int port)
{
	BingoReceiver *receiver = new BingoReceiver(port);
	BingoClient *client = new BingoClient(receiver);
	BingoSender *sender = new BingoSender(client);

	//OpenGame();

	BoardWindow *bw = new BoardWindow(sender, receiver);
	bw->show();

	/*
	connect(bw, &QMainWindow::closeEvent, [this]() {
		this->show();
	}); //listen for game window to close to unhide this window

	this->hide();
	*/
}

/*
 * This connects to the main game server to send and receive data about rooms between players
 */
void MainClient::ConnectToServer()
{
	std::string ipAddress = IP_ADDR;			// ip address of the server
	int port = PORT_NUMBER;						// listening port on the server

	WSAData wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &wsData); 	//initialize winsock
	if (wsResult != 0) {
		qDebug() << "CLIENT: Can't intialize WinSock";
		return;
	}

	m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_serverSocket == INVALID_SOCKET) {
		qDebug() << "CLIENT: Error creating socket";
		WSACleanup();
		return;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	int connectionResult = ::connect(m_serverSocket, (sockaddr *)&hint, sizeof(hint));
	if (connectionResult == SOCKET_ERROR) {
		qDebug() << "CLIENT: Cannot connect to server";
		closesocket(m_serverSocket);
		WSACleanup();
		return;
	}

	qDebug() << "CLIENT: Connected to server";
	//Listen(); //Now listen for incoming requests to update room list

	return;
}

void MainClient::OpenGame()
{
	setlocale(LC_CTYPE, "");

	// additional information
	LPCTSTR lpApplicationName = L"C:/Users/Alec Grant/Downloads/Dolphin-x64/Dolphin.exe";
	const char *text =
		"Dolphin.exe --exec \"C:/Users/Alec Grant/Downloads/sms_ntsc-u/Super Mario Sunshine (USA).iso\"";
	size_t wn = mbsrtowcs(NULL, &text, 0, NULL);
	wchar_t * buf = new wchar_t[wn + 1]();
	wn = mbsrtowcs(buf, &text, wn + 1, NULL);
	LPWSTR params = buf;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// start Dolphin
	if (CreateProcess(
		lpApplicationName,		// the path
		params,					// Command line
		NULL,					// Process handle not inheritable
		NULL,					// Thread handle not inheritable
		FALSE,					// Set handle inheritance to FALSE
		0,						// No creation flags
		NULL,					// Use parent's environment block
		NULL,					// Use parent's starting directory 
		&si,					// Pointer to STARTUPINFO structure
		&pi						// Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	)) {
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

}

void MainClient::Listen()
{
	char buf[BUFFER_SIZE];

	while (true) {
		memset(buf, 0, BUFFER_SIZE);
		int received = recv(m_serverSocket, buf, BUFFER_SIZE, 0);

		//do a check here
		int flag;
		sscanf(buf, "%d", &flag);
		
		qDebug() << "CLIENT( " << m_username << "): Received flag from server: " << flag;

		if ((ServerFlag)flag == ServerFlag::RequestJoin) { // connecting to other player
			memset(buf, 0, BUFFER_SIZE);
			int received = recv(m_serverSocket, buf, BUFFER_SIZE, 0);
			char username[BUFFER_SIZE];
			sscanf(buf, "%s", username);
			
			qDebug() << "CLIENT(" << m_username << "): Request to join received by " << username;
			emit IncomingRequest(username);
		}
		else if((ServerFlag)flag == ServerFlag::RefreshRooms) { // refresh room list	
			memset(buf, 0, BUFFER_SIZE);
			int received = recv(m_serverSocket, buf, BUFFER_SIZE, 0);
			int roomSize;
			sscanf(buf, "%d", &roomSize);

			qDebug() << "CLIENT(" << m_username << "): Refreshed Room List[" << roomSize << "]";

			m_roomList = new QString[roomSize];
			char buf[BUFFER_SIZE];

			int count = 0;
			for (int i = 0; i < roomSize; i++) {
				memset(buf, 0, BUFFER_SIZE);

				int received = recv(m_serverSocket, buf, BUFFER_SIZE, 0);
				//do a check here
				
				qDebug() << "CLIENT(" << m_username << "): Received room by " << buf;
				m_roomList[count] = (QString)buf;
				count++;
			}
			emit Refresh(roomSize);
		}
	}
}

void MainClient::RefreshRoomList(int roomSize)
{
	//looking for new rooms to add that aren't in our current room list
	for (int i = 0; i < roomSize; i++) {
		if (m_roomList[i] != m_username) {
			if (m_roomArray.size() == 0) {
				qDebug() << "CLIENT(" << m_username << "): Room by " << m_roomList[i] << " added.";
				RoomWidget *roomWidget = new RoomWidget(1, m_roomList[i], m_roomWidget);
				m_roomLayout->addWidget(roomWidget);
				m_roomLayout->setAlignment(roomWidget, Qt::AlignTop);
				
				m_roomArray.append(roomWidget);
				connect(roomWidget, &RoomWidget::RequestToJoin, this, &MainClient::SendRequest);
			}
			else {
				for (int j = 0; j < m_roomArray.size(); j++) {
					if (m_roomList[i] != m_roomArray[j]->GetUsername()) {
						qDebug() << "CLIENT(" << m_username << "): " << m_roomList[i] 
							<< " != " << m_roomArray[j]->GetUsername();
						qDebug() << "CLIENT(" << m_username << "): Room by " 
							<< m_roomList[i] << " added.";
						RoomWidget *roomWidget = new RoomWidget(1, m_roomList[i], m_roomWidget);
						m_roomLayout->addWidget(roomWidget);
						m_roomLayout->setAlignment(roomWidget, Qt::AlignTop);
						
						m_roomArray.append(roomWidget);
						connect(roomWidget, &RoomWidget::RequestToJoin, this, &MainClient::SendRequest);
					}
					else {
						qDebug() << "CLIENT(" << m_username << "): Room by " 
							<< m_roomArray[j]->GetUsername() << " already in list.";
						break;
					}
				}
			}
		}
		else {
			qDebug() << "CLIENT(" << m_username << "): Room by " 
				<< m_roomList[i] << " rejected (same username).";
		}
	}
}

void MainClient::SendRequest(RoomWidget *roomWidget)
{
	char buf[BUFFER_SIZE];
	memset(buf, 0, BUFFER_SIZE);
	sprintf(buf, "%d", ServerFlag::RequestJoin);
	send(m_serverSocket, buf, BUFFER_SIZE, 0); //send the request to join flag

	//then send the username to connect to
	memset(buf, 0, BUFFER_SIZE);
	sprintf(buf, "%s", roomWidget->GetUsername().toLocal8Bit().data());
	send(m_serverSocket, buf, BUFFER_SIZE, 0);

	qDebug() << "CLIENT(" << m_username << "): requesting to join room by "
		<< roomWidget->GetUsername();

	//receive answer (accept or reject)
	memset(buf, 0, BUFFER_SIZE);
	int received = recv(m_serverSocket, buf, BUFFER_SIZE, 0);
	ServerFlag flag;
	sscanf(buf, "%d", &flag);

	if (flag == ServerFlag::RequestAccept) {
		qDebug() << "CLIENT(" << m_username << "): Request to join " 
			<< roomWidget->GetUsername() << " accepted.";
		
		//get port number from other user
		memset(buf, 0, BUFFER_SIZE);
		int received = recv(m_serverSocket, buf, BUFFER_SIZE, 0);
		int port;
		sscanf(buf, "%d", &port);
		
		ConnectToRemoteRoom(port);
	}
	else if (flag == ServerFlag::RequestReject) {
		qDebug() << "CLIENT(" << m_username << "): Request to join " 
			<< roomWidget->GetUsername() << " rejected.";
	}
}
