#include<iostream>
#include<pthread.h>
#include<thread>
#include<sys/epoll.h>
#include <cstring>
#include<unistd.h>
#include<fcntl.h>
#include<cstring>
#include<stdio.h>
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
    sendClientLogin(getmClientName(mClientName));
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
    //return (mail.data[0] == '/') ? true : false;
}

//-----------------------------------------------------------------------------
void Client::processingInputCommand(Mail& mail)
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
char* Client::getmClientName(char* name)
//-----------------------------------------------------------------------------
{
    cout << "Enter Name" << endl;
    cin >> name;
    return name;
}

//-----------------------------------------------------------------------------
void Client::getClientPassword(char* name)
//-----------------------------------------------------------------------------
{
    cout << "Enter password" << endl;
    cin >> name;
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
        return false;
    } else {
        cout << "Client::createSocket: mSocket = "<< mSocket <<" was created" << endl;
        return true;
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
    if (0 == strcmp(mail.data, mClientName)) {
        cout << "Log in to the server with a nickname: " << mClientName << endl;
    } else{
        memset(mClientName, 0,sizeof(mClientName));
        cout << mail.data << endl;
        sendClientLogin(getmClientName(mClientName));
    }
    /*
    if(mail.data[0] != 0){
        cout << "Log in to the server with a nickname: " << mClientName << endl;
    } else{
        memset(mClientName, 0,sizeof(mClientName));
        cout << "Name not available"<< endl;
        sendClientLogin(getmClientName(mClientName));
    }
    */
}
