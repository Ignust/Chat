#include "client.hpp"
#include<iostream>
#include<pthread.h>
#include<thread>

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
    thread sendThread(sendMessenger,mSocket);
    thread checkThread(checkMessenger,mSocket);
    sendThread.join();
    checkThread.join();
}
/////////////////////////////////////////////////////////////////////////////////////////
bool Client::disconnectClient()
/////////////////////////////////////////////////////////////////////////////////////////
{

}

/////////////////////////////////////////////////////////////////////////////////////////
void Client::sendMessenger(int soket)
/////////////////////////////////////////////////////////////////////////////////////////
{
    //Client& mClient = (Client&)client;

    Mail tempMail;
    tempMail.typeMail = MESSAGE;
    while (true) {
        cout << "Enter messenger" << endl;
        cin >> tempMail.data;
        send(soket, &tempMail ,sizeof (Mail), 0);
    }

    /*
    Mail tempMail;
    tempMail.typeMail = MESSAGE;
    cin >> tempMail.data;
    send(soket, &tempMail ,sizeof (Mail), 0);
    */

}
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
