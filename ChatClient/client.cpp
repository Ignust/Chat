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
        sendMessenger();
    }
}

//-----------------------------------------------------------------------------
void Client::disconnectClient()
//-----------------------------------------------------------------------------
{

}

//-----------------------------------------------------------------------------
void Client::sendMessenger()
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
        tempMail.typeMail = MESSAGE;
        read(events[a].data.fd, tempMail.data,sizeof(tempMail.data));
        //cout << "read" << endl;
        if (tempMail.data[0]!=0) {
            //cout <<" tempMail.data: " << tempMail.data << endl;
            send(mSocket, &tempMail, sizeof (Mail), 0);
        }

    }
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
        //memset(tempMail.data, 0, sizeof(tempMail.data));
        //read(events[i].data.fd, tempMail.data, sizeof (tempMail.data));
        if(0 <= read(events[i].data.fd, &tempMail, sizeof (Mail))){
            processMailType(tempMail);
        }
        //if (tempMail.data[0] != 0) {
            //cout << "recv: " << tempMail.data << flush;
        //}

    }
}

//-----------------------------------------------------------------------------
char* Client::getmClientName( char* name)
//-----------------------------------------------------------------------------
{
    //static char name[1024];
    cout << "Enter Name" << endl;
    cin >> name;
    cout << "Client::getmClientName: name = " << name << endl;
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
    send(mSocket, &tempMail, sizeof (Mail), 0);
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
        cout << "recv: " << mail.data << flush;
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
        sendClientName(getmClientName(mClientName));
        cout << "Name not available"<< endl;
    }
}
