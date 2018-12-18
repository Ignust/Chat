#include<iostream>
#include<pthread.h>
#include<thread>
#include<sys/epoll.h>
#include <cstring>
#include<unistd.h>
#include<fcntl.h>
#include<cstring>
using namespace std;

#include "Client.hpp"

//-----------------------------------------------------------------------------
Client::Client()
//-----------------------------------------------------------------------------
    : mSocket(0)
    , mSockAddr()
    , mSet()
    , mMail()
{
    memset(mClientName, 0, sizeof (mClientName));
    if (initClient()) {
        cout << "Client::Client: Client init" << endl;
    } else {
        cout << "Client::Client: Client was not init" << endl;
    }
}

//-----------------------------------------------------------------------------
bool Client::initClient()
//-----------------------------------------------------------------------------
{
    if (mSocket != 0) {
        return true;
    }
    if (createSocket() && connectSocket()) {
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
void Client::start[[noreturn]]()
//-----------------------------------------------------------------------------
{
    sendClientName(getmClientName(mClientName));
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
void Client::sendMail(Mail& mail)
//-----------------------------------------------------------------------------
{
    send (mSocket, &mail, sizeof (Mail), 0);
}

//-----------------------------------------------------------------------------
bool Client::checkInputCommand(Mail& mail)
//-----------------------------------------------------------------------------
{
    if (mail.data[0] == '/') {
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
void Client::processingInputCommand(Mail& mail)
//-----------------------------------------------------------------------------
{
    if (0 == strcmp ("/help\n", mail.data) ) {
        processingCommandHelp();
        return;
    }
    if (0 == strcmp ("/disconnect server\n", mail.data) ) {
        processingCommandDisconnectServer();
        return;
    }

    cout <<"Wrong command, enter /help for to get more information about commands" << endl;
}

//-----------------------------------------------------------------------------
void Client::processingCommandHelp()
//-----------------------------------------------------------------------------
{
    cout << "Client::processingCommandHelp" << endl;
}

//-----------------------------------------------------------------------------
void Client::processingCommandDisconnectServer()
//-----------------------------------------------------------------------------
{
    cout << "Client::processingCommandDisconnectServer" << endl;
    Mail tempMail;
    tempMail.typeMail = DISCONNECT_SERVER;
    sendMail(tempMail);
}

//-----------------------------------------------------------------------------
void Client::checkMessenger()
//-----------------------------------------------------------------------------
{
    int epollFd = epoll_create1(0);

    epoll_event eventFd;
    epoll_event events[AMOUNT_EVENTS];
    eventFd.events = EPOLLIN;
    eventFd.data.fd = mSocket;
    epoll_ctl(epollFd, EPOLL_CTL_ADD, mSocket, &eventFd);

    int amountEvents = epoll_wait(epollFd, events, AMOUNT_EVENTS, 500);
    for (int i = 0; i < amountEvents; ++i) {
        Mail tempMail;
        if(0 <= read(events[i].data.fd, &tempMail, sizeof (Mail))){
            processMailType(tempMail);
        }
    }
}

//-----------------------------------------------------------------------------
char* Client::getmClientName( char* name)
//-----------------------------------------------------------------------------
{
    cout << "Enter Name" << endl;
    cin >> name;
    return name;
}

//-----------------------------------------------------------------------------
void Client::sendClientName(char * name)
//-----------------------------------------------------------------------------
{
    Mail tempMail;
    tempMail.typeMail = CLIENT_LOGIN;
    memset(tempMail.data, 0 ,sizeof (tempMail.data));
    strncpy(tempMail.data, name, sizeof (mClientName));
    sendMail(tempMail);
}

//-----------------------------------------------------------------------------
bool Client::createSocket()
//-----------------------------------------------------------------------------
{
    mSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mSocket < 2) {
        cout << "ERORR: Client::createSocket: mSocket = "<< mSocket << endl;
        return false;
    } else {
        cout << "Client::createSocket: mSocket = "<< mSocket <<" was created" << endl;
    }
    //fcntl(mSocket, F_SETFL, O_NONBLOCK);
}

//-----------------------------------------------------------------------------
bool Client::connectSocket()
//-----------------------------------------------------------------------------
{
    mSockAddr.sin_family = AF_INET;
    mSockAddr.sin_port = htons(PORT_ADDR);
    mSockAddr.sin_addr.s_addr = htons(INADDR_ANY);

    if (connect(mSocket,(sockaddr*)(&mSockAddr), sizeof (sockaddr)) < 0) {
        cout << "ERORR: Client::connectSocket" << endl;
        return false;
    } else {
        cout << "Client::connectSocket: mSocket is connecting" << endl;
        return true;
    }
}

//-----------------------------------------------------------------------------
void Client::processMailType(Mail& mail)
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
    default:
        cout << "ERROR: Client::processMailType: mail.typeMail = "<< mail.typeMail << endl;
    }
}

//-----------------------------------------------------------------------------
void Client::processMailMessage(Mail& mail)
//-----------------------------------------------------------------------------
{
    if (mail.data[0] != 0) {
        cout << mail.data << flush;
    }
}

//-----------------------------------------------------------------------------
void Client::processMailCommand(Mail& mail)
//-----------------------------------------------------------------------------
{
    cout << "Client::processMailCommand:" << mail.data << endl;
}

//-----------------------------------------------------------------------------
void Client::processMailClientLOgin(Mail& mail)
//-----------------------------------------------------------------------------
{
    if(mail.data[0] != 0){
        cout << "Log in to the server with a nickname: " << mClientName << endl;
    } else{
        memset(mClientName, 0,sizeof(mClientName));
        cout << "Name not available"<< endl;
        sendClientName(getmClientName(mClientName));
    }
}
