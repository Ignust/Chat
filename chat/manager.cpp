#include "manager.hpp"
#include"mail.hpp"
#include<iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include<cstring>

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
    Client tempClient;
    tempClient.clientId = newClient;
    mClients.push_back(tempClient);
}
//------------------------------------------------------------------------------------------
void Manager::popClient(int ClientId)
//------------------------------------------------------------------------------------------
{
    cout << "erase: Client = " << ClientId << endl;
    for(list<Client>::iterator it = mClients.begin(); it != mClients.end(); it++){
        if(it->clientId == ClientId){
            mClients.erase(it);
        }
    }
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
//------------------------------------------------------------------------------------------
{
    //cout << "getClient()" << endl;
    list<Client>::iterator it = mClients.begin();
    //cout << "it = " << *it << endl;
    while (number != 0) {
        it++;
        number--;
        //cout << "it = " << *it << endl;
    }

    return it->clientId;
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
    for(list<Client>::iterator it = mClients.begin(); it != mClients.end(); it++){
        if(it->clientId != mail.clientId /*&& it->clientName != NULL*/){
            cout <<"send mail " << "from: " << it->clientId <<": " << mail.data << endl;
            send(it->clientId, mail.data, sizeof (mail.data), 0);
        }
    }
}
//------------------------------------------------------------------------------------------
void Manager::processMailCommand(Mail& mail)
//------------------------------------------------------------------------------------------
{
    if (checkNewClientName(mail)){
        for(list<Client>::iterator it = mClients.begin(); it != mClients.end(); it++){
            if(it->clientId == mail.clientId){
                strncpy(it->clientName, mail.data, sizeof (mail.data));
                cout << "ClientId: " << it->clientId << " ClientName: " << it->clientName << endl;
            }
        }
    }else {
        cout << "Fail add clientName to " << mail.clientId << endl;
}
}
//------------------------------------------------------------------------------------------
bool Manager::checkNewClientName(Mail& mail)
//------------------------------------------------------------------------------------------
{
    for(list<Client>::iterator it = mClients.begin(); it != mClients.end(); it++){
        if(0 == strcmp(it->clientName, mail.data) ){
            return false;
        }
    }
    return true;
}

