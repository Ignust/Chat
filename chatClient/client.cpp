#include "client.hpp"
#include<iostream>
#include<pthread.h>
#include<thread>
#include<sys/epoll.h>
#include <cstring>
#include<unistd.h>

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////
Client::Client()
/////////////////////////////////////////////////////////////////////////////////////////
    : mSocket(0)
    , mSockAddr()
    , mSet()
    , mMail()
{
    if(initClient()){
        cout << "       Client init" << endl;
    }else{
        cout << "       Client was not init" << endl;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////
bool Client::initClient()
/////////////////////////////////////////////////////////////////////////////////////////
{
    if(mSocket != 0){
        return true;
    }
    mSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(mSocket < 2){
        cout << "Erorr mSocket" << endl;
        return false;
    }else{
        cout << "mSocket was created" << endl;
    }
    //fcntl(mSocket, F_SETFL, O_NONBLOCK);

    mSockAddr.sin_family = AF_INET;
    mSockAddr.sin_port = htons(PORT_ADDR);
    mSockAddr.sin_addr.s_addr = htons(INADDR_ANY);

    if(connect(mSocket,(sockaddr*)(&mSockAddr), sizeof (sockaddr)) < 0){
        cout << "Erorr connect()" << endl;
    }else{
        cout << "mSocket is connecting" << endl;
    }
    return true;

}
/////////////////////////////////////////////////////////////////////////////////////////
void Client::start()
/////////////////////////////////////////////////////////////////////////////////////////
{
    while (true) {
        checkMessenger();
        sendMessenger();


    }



    /*
    thread sendThread(sendMessenger,mSocket);
    thread checkThread(checkMessenger,mSocket);recv()
    sendThread.join();
    checkThread.join();
    */
}
/////////////////////////////////////////////////////////////////////////////////////////
bool Client::disconnectClient()
/////////////////////////////////////////////////////////////////////////////////////////
{

}

/////////////////////////////////////////////////////////////////////////////////////////
void Client::sendMessenger()
/////////////////////////////////////////////////////////////////////////////////////////
{
    int epollFd = epoll_create1(0);

    epoll_event eventFd;
    epoll_event events[20];
    eventFd.events = EPOLLIN;
    eventFd.data.fd = 0;
    epoll_ctl(epollFd, EPOLL_CTL_ADD, 0, &eventFd);
    int amountEvents = epoll_wait(epollFd, events, 20, -1);
    for(int a = 0; a < amountEvents; ++a){
        Mail tempMail;
        memset(tempMail.data, 0, sizeof(tempMail.data));
        tempMail.data[0] = 0;
        tempMail.typeMail = MESSAGE;
        read(events[a].data.fd, tempMail.data,sizeof(tempMail.data));

        if(tempMail.data[0]!=0){
            cout <<" tempMail.data: " << tempMail.data << endl;
            send(mSocket, &tempMail, sizeof (tempMail), 0);
        }

    }
}
/*
/////////////////////////////////////////////////////////////////////////////////////////
void Client::sendMessenger(int soket)
/////////////////////////////////////////////////////////////////////////////////////////
{
    Mail tempMail;
    tempMail.typeMail = MESSAGE;
    while (true) {
        cout << "Enter messenger" << endl;
        cin >> tempMail.data;
        send(soket, &tempMail ,sizeof (Mail), 0);
    }

}
*/
/////////////////////////////////////////////////////////////////////////////////////////
void Client::checkMessenger()
/////////////////////////////////////////////////////////////////////////////////////////
{
    int epollFd = epoll_create1(0);

    epoll_event eventFd;
    epoll_event events[20];
    eventFd.events = EPOLLIN;
    eventFd.data.fd = mSocket;
    epoll_ctl(epollFd, EPOLL_CTL_ADD, mSocket, &eventFd);
    int amountEvents = epoll_wait(epollFd, events, 20, -1);
    for(int i = 0; i < amountEvents; ++i){
        Mail tempMail;
        recv(events[i].data.fd, tempMail.data, sizeof (tempMail.data), 0);
        if(tempMail.data[0] != 0){
            cout << "recv: " << tempMail.data << endl;
        }

    }
}
/*
/////////////////////////////////////////////////////////////////////////////////////////
void Client::checkMessenger(int mSocket)
/////////////////////////////////////////////////////////////////////////////////////////
{
    Mail tempMail;
    while (1) {
        recv(mSocket, tempMail.data, sizeof (tempMail.data), 0);
        cout << "recv: " << tempMail.data << endl;
    }
}
*/
