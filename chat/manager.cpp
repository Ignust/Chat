#include "manager.hpp"
#include"mail.hpp"
#include<iostream>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;


//------------------------------------------------------------------------------------------
Manager::Manager()
//------------------------------------------------------------------------------------------
    : mClients()
    , mMails()
{

}
//------------------------------------------------------------------------------------------
void Manager::pushClient(int newClient)
//------------------------------------------------------------------------------------------
{
    char welcomeMsg[] = "Welcom to the Chat Serve";
    send(newClient, &welcomeMsg, sizeof (welcomeMsg), 0);
    cout << "newClient = " << newClient << endl;
    mClients.insert(newClient);
}
//------------------------------------------------------------------------------------------
void Manager::popClient(int Client)
//------------------------------------------------------------------------------------------
{
    cout << "erase: Client = " << Client << endl;
    mClients.erase(Client);
}
//------------------------------------------------------------------------------------------
bool Manager::pushMail(int client)
//------------------------------------------------------------------------------------------
{
    Mail tempMail;
    int bytesRecv;
    bytesRecv = recv(client,&tempMail,sizeof (Mail),0);
    if(bytesRecv <= 0){
        //Drop the client
        return false;
    }else {
        tempMail.clientId = client;
        mMails.push(tempMail);
        return true;
}

}
//------------------------------------------------------------------------------------------
void Manager::processMails()
//------------------------------------------------------------------------------------------
{
    if(!mMails.empty()){
        processMailType(mMails.front());
        mMails.pop();
    }else{
        //cout <<"Mail empty" << endl;
    }
}
//------------------------------------------------------------------------------------------
int Manager::getAmountOfClient()
//------------------------------------------------------------------------------------------
{
    return mClients.size();
}
//------------------------------------------------------------------------------------------
int Manager::getClient(int number)
{
    //cout << "getClient()" << endl;
    set<int>::iterator it = mClients.begin();
    //cout << "it = " << *it << endl;
    while (number != 0) {
        it++;
        number--;
        //cout << "it = " << *it << endl;
    }

    return *(it);
}
//------------------------------------------------------------------------------------------
void Manager::processMailType(Mail& mail)
//------------------------------------------------------------------------------------------
{
    switch (mail.typeMail) {
    case MESSAGE:
        processMailMessage(mail);
        break;
    case COMMAND:
        processMailCommand(mail);
        break;
    default:
        cout << "Erorr processMailType() " << endl;
        break;
    }
}
//------------------------------------------------------------------------------------------
void Manager::processMailMessage(Mail& mail)
//------------------------------------------------------------------------------------------
{
    //cout <<"send mail : " << mail.data << endl;
    for(set<int>::iterator it = mClients.begin(); it != mClients.end(); it++){
        if((*it) != mail.clientId){
            cout <<"send mail " << "from: " << *it <<": " << mail.data << endl;
            send(*it, mail.data, sizeof (mail.data), 0);
        }
    }
}
//------------------------------------------------------------------------------------------
void Manager::processMailCommand(Mail& mail)
//------------------------------------------------------------------------------------------
{

}


