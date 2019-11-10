#include <iostream>
#include <pthread.h>
#include <thread>
#include <sys/epoll.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <stdio.h>

using namespace std;

#include "Client.hpp"
#include "MailTypes.hpp"

//-----------------------------------------------------------------------------
Client::Client()
    : mSocket(0)
    , mSet()
    , mOutput(false)
//-----------------------------------------------------------------------------
{
    memset(mClientName, 0, sizeof (mClientName));
    if (!initClient()) {
        cout << "Client::Client: Client was not init" << endl;
    }
}

//-----------------------------------------------------------------------------
bool Client::initClient()
//-----------------------------------------------------------------------------
{
    return createSocket() && connectSocket();
}

//-----------------------------------------------------------------------------
void Client::start[[noreturn]]()
//-----------------------------------------------------------------------------
{
    sendClientLogin(getmClientName());

    while (true) {
        checkMessenger();
        checkKeyboardInput();
    }
}

//-----------------------------------------------------------------------------
void Client::disconnectClient()
//-----------------------------------------------------------------------------
{

}

//-----------------------------------------------------------------------------
void Client::checkKeyboardInput()
//-----------------------------------------------------------------------------
{
    int epollFd = epoll_create1(0);

    epoll_event eventFd;
    epoll_event events[AMOUNT_EVENTS];
    eventFd.events = EPOLLIN;
    eventFd.data.fd = 0;
    epoll_ctl(epollFd, EPOLL_CTL_ADD, 0, &eventFd);

    int amountEvents = epoll_wait(epollFd, events, AMOUNT_EVENTS, 500);
    for (int a = 0; a < amountEvents; ++a) {
        Mail tempMail;
        memset(tempMail.data, 0, sizeof(tempMail.data));
        read(events[a].data.fd, tempMail.data,sizeof(tempMail.data));
        if (tempMail.data[0]!=0) {
            if (checkInputCommand(tempMail)){
                processingInputCommand(tempMail);
            } else {
                tempMail.typeMail = MESSAGE;
                sendMail(tempMail);
            }
        }
    }
}

//-----------------------------------------------------------------------------
void Client::sendMail(const Mail& mail)
//-----------------------------------------------------------------------------
{
    send(mSocket, &mail, sizeof (Mail), 0);
}

//-----------------------------------------------------------------------------
bool Client::checkInputCommand(const Mail& mail)
//-----------------------------------------------------------------------------
{
    return (mail.data[0] == '/');
}

//-----------------------------------------------------------------------------
void Client::processingInputCommand( const Mail& mail)
//-----------------------------------------------------------------------------
{
    if (0 == strcmp ("/help\n", mail.data) ) {
        processingCommandHelp();
        return;
    }
    if (0 == strcmp ("/ds\n", mail.data) ) {
        processingCommandDisconnectServer();
        return;
    }
    if (0 == strcmp ("/dc\n", mail.data) ) {
        processingCommandDisconnectClient();
        return;
    }

    cout <<"Wrong command, enter /help for to get more information about commands" << endl;
}

//-----------------------------------------------------------------------------
void Client::processingCommandHelp()
//-----------------------------------------------------------------------------
{
    cout << "----------CommandHelp-------------" << endl;

    cout << "/ds - disconnect server" << endl;
    cout << "/dc - disconnect client" << endl;

    cout << "----------------------------------" << endl;
}

//-----------------------------------------------------------------------------
void Client::processingCommandDisconnectServer()
//-----------------------------------------------------------------------------
{
    //cout << "Client::processingCommandDisconnectServer" << endl;
    Mail tempMail;
    tempMail.typeMail = DISCONNECT_SERVER;
    sendMail(tempMail);
}

//-----------------------------------------------------------------------------
void Client::processingCommandDisconnectClient()
//-----------------------------------------------------------------------------
{
    cout << "Enter name" << endl;

    int epollFd = epoll_create1(0);

    epoll_event eventFd;
    epoll_event events[1];
    eventFd.events = EPOLLIN;
    eventFd.data.fd = 0;
    epoll_ctl(epollFd, EPOLL_CTL_ADD, 0, &eventFd);
    bool waitingForInput = true;
    while (waitingForInput) {
        int amountEvents = epoll_wait(epollFd, events, 1, 500);
        for (int a = 0; a < amountEvents; ++a) {
            Mail tempMail;
            memset(tempMail.data, 0, sizeof(tempMail.data));
            read(events[a].data.fd, tempMail.data,sizeof(tempMail.data));
            if (tempMail.data[0]!=0) {
                tempMail.typeMail = DISCONNECT_CLIENT;
                tempMail.data[strlen(tempMail.data)-1] = 0;
                sendMail(tempMail);
                waitingForInput = false;
                //cout << "Client::processingCommandDisconnectClient:sendMail" << endl;
            }
        }
        checkMessenger();
    }

    }

//-----------------------------------------------------------------------------
void Client::checkMessenger()
//-----------------------------------------------------------------------------
{
    int epollFd = epoll_create1(0);

    epoll_event eventFd = {};
    epoll_event events[AMOUNT_EVENTS] = {};
    eventFd.events = EPOLLIN;
    eventFd.data.fd = mSocket;
    epoll_ctl(epollFd, EPOLL_CTL_ADD, mSocket, &eventFd);

    int amountEvents = epoll_wait(epollFd, events, AMOUNT_EVENTS, 500);
    for (int i = 0; i < amountEvents; ++i) {
        Mail tempMail = {};
        if(0 <= read(events[i].data.fd, &tempMail, sizeof (Mail))){
            processMailType(tempMail);
        }
    }
}

//-----------------------------------------------------------------------------
char* Client::getmClientName()
//-----------------------------------------------------------------------------
{
    cout << "Enter Name" << endl;
    cin >> mClientName;
    return mClientName;
}

//-----------------------------------------------------------------------------
void Client::getClientPassword(char* password)
//-----------------------------------------------------------------------------
{
    cout << "Enter password" << endl;
    cin >> password;
}

//-----------------------------------------------------------------------------
void Client::sendClientLogin(char * name)
//-----------------------------------------------------------------------------
{
    char clientPassword[ARRAY_SIZE] = {};
    getClientPassword(clientPassword);
    char clientLogin[ARRAY_SIZE * 2] = {};
    snprintf(clientLogin, sizeof (clientLogin),"%s %s", name, clientPassword);

    Mail tempMail;
    tempMail.typeMail = CLIENT_LOGIN;
    memset(tempMail.data, 0 ,sizeof (tempMail.data));
    strncpy(tempMail.data, clientLogin, sizeof (mClientName));
    sendMail(tempMail);
}

//-----------------------------------------------------------------------------
bool Client::createSocket()
//-----------------------------------------------------------------------------
{
    mSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mSocket < 2) {
        cout << "ERORR: Client::createSocket: mSocket = "<< mSocket << endl;
    }

    return (mSocket >= 2);
}

//-----------------------------------------------------------------------------
bool Client::connectSocket()
//-----------------------------------------------------------------------------
{
    struct sockaddr_in socketAddress{};
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_port = htons(PORT_ADDR);
    socketAddress.sin_addr.s_addr = htons(INADDR_ANY);

    const bool connectResult = ( 0 <= connect(mSocket,(sockaddr*)(&socketAddress), sizeof (sockaddr)) );
    if (!connectResult) {
        cout << "ERORR: Client::connectSocket" << endl;
    }

    return connectResult;
}

//-----------------------------------------------------------------------------
void Client::processMailType(const Mail& mail)
//-----------------------------------------------------------------------------
{
    switch (mail.typeMail) {
    case MESSAGE:
        processMailMessage(mail);
        break;
    case COMMAND:
        processMailCommand(mail);
        break;
    case CLIENT_LOGIN:
        processMailClientLOgin(mail);
        break;
    case DISCONNECT_CLIENT:
        processMailDisconnectClient(mail);
        break;
    default:
        cout << "ERROR: Client::processMailType: mail.typeMail = "<< mail.typeMail << endl;
    }
}

//-----------------------------------------------------------------------------
void Client::processMailMessage(const Mail& mail)
//-----------------------------------------------------------------------------
{
    if (mail.data[0] != 0) {
        cout << mail.data << flush;
    }
}

//-----------------------------------------------------------------------------
void Client::processMailCommand(const Mail& mail)
//-----------------------------------------------------------------------------
{
    cout << "Client::processMailCommand:" << mail.data << endl;
}

//-----------------------------------------------------------------------------
void Client::processMailClientLOgin(const Mail& mail)
//-----------------------------------------------------------------------------
{
    if (0 == strcmp(mail.data, mClientName)) {
        cout << "Log in to the server with a nickname: " << mClientName << endl;
    } else{
        memset(mClientName, 0,sizeof(mClientName));
        cout << mail.data << endl;
        sendClientLogin(getmClientName());
    }
}

//-----------------------------------------------------------------------------
void Client::processMailDisconnectClient(const Mail& mail)
//-----------------------------------------------------------------------------
{
    if (mail.data[0] != 0) {
        cout << mail.data << flush;
    }

    if (createSocket() && connectSocket()) {
        sendClientLogin(getmClientName());
    }
}
