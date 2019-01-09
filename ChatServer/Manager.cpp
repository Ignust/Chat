#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include<stdio.h>//snprintf();
#include <iostream>
using std::cout;
using std::endl;
using std::flush;


#include "Manager.hpp"
#include"EventHandler.hpp"

//------------------------------------------------------------------------------------------
Manager::Manager(EventHandler& eventHandler)
    : mClients()
    , mMails()
    , mEvHndlr(eventHandler)
    , mDatabase()
//------------------------------------------------------------------------------------------
{

}

//------------------------------------------------------------------------------------------
void Manager::pushClient(int newClient)
//------------------------------------------------------------------------------------------
{
    cout << "Manager::pushClient: newClient = " << newClient << endl;
    Client tempClient;
    tempClient.clientId = newClient;
    mClients.push_back(tempClient);
}

//------------------------------------------------------------------------------------------
void Manager::popClient(int ClientId)
//------------------------------------------------------------------------------------------
{
    cout << "Manager::popClient: ClientId = " << ClientId;
    for (list<Client>::iterator it = mClients.begin(); it != mClients.end(); ++it) {
        if (it->clientId == ClientId) {
            it = mClients.erase(it);
            cout << " was erased" << endl;
            return;
        }
    }
}

//------------------------------------------------------------------------------------------
bool Manager::pushMail(int client)
//------------------------------------------------------------------------------------------
{
    Mail tempMail;
    int bytesRecv = static_cast<int>( recv(client, &tempMail, sizeof(Mail), 0) );
    if (bytesRecv <= 0) {
        cout << "Manager::pushMail: bytesRecv <= 0" << endl;
        //Drop the client
        return false;
    } else {
        tempMail.clientId = client;
        mMails.push(tempMail);
        return true;
    }
}

//------------------------------------------------------------------------------------------
void Manager::processMails()
//------------------------------------------------------------------------------------------
{
    if (!mMails.empty()) {
        processMailType(mMails.front());
        mMails.pop();
    }
}

//------------------------------------------------------------------------------------------
int Manager::getAmountOfClient() const
//------------------------------------------------------------------------------------------
{
    return static_cast<int>(mClients.size());
}

//------------------------------------------------------------------------------------------
int Manager::getClient(int number)
//------------------------------------------------------------------------------------------
{
    list<Client>::iterator it = mClients.begin();
    while (number != 0) {
        it++;
        number--;
    }
    return it->clientId;
}

//------------------------------------------------------------------------------------------
void Manager::sendMail(Mail& meil, int clietn)
//------------------------------------------------------------------------------------------
{
    send(clietn, &meil, sizeof (Mail), 0);
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
        //
        break;
    case CLIENT_LOGIN:
        processMailClientLogin(mail);
        break;
    case DISCONNECT_SERVER:
        processMailDisconnectServer(mail);
        break;
    case DISCONNECT_CLIENT:
        processMailDisconnectClient(mail);
        break;
    default:
        cout << "ERROR: Manager::processMailType: mail.typeMail = "<< mail.typeMail << endl;
    }
}

//------------------------------------------------------------------------------------------
void Manager::processMailMessage(Mail& mail)
//------------------------------------------------------------------------------------------
{
    char* clientNameOfMail = getClietName(mail.clientId);
    clientNameOfMail = getClietName(mail.clientId);
    for (auto it = mClients.begin(); it != mClients.end(); ++it) {
        if (it->clientId != mail.clientId && it->clientName[0] != 0) {
            Mail tempMail;
            tempMail.typeMail = MESSAGE;
            snprintf (tempMail.data,sizeof (tempMail.data), "[ %s ]: %s", clientNameOfMail
                      , mail.data);
            cout <<"Manager::processMailMessage: id = " << it->clientId <<"; data: "
                << mail.data << flush;
            sendMail(tempMail, it->clientId);
        }
    }
}

//------------------------------------------------------------------------------------------
void Manager::processMailClientLogin(Mail& mail)
//------------------------------------------------------------------------------------------
{
    char clientName[1024] = {};
    char clientPassword[1024] = {};
    parseClientLogin(clientName, clientPassword,mail);

    if (checkNewClientName(clientName)) {
        for (auto it = mClients.begin(); it != mClients.end(); ++it) {
            if (it->clientId == mail.clientId) {
                strncpy(it->clientName, clientName, sizeof (clientName));
                strncpy(it->ClientPassword, clientPassword, sizeof (clientPassword));
                it->ClientLvl = 0;
                Mail tempMail;
                tempMail.typeMail = CLIENT_LOGIN;
                strcpy(tempMail.data, it->clientName);
                sendMail(tempMail,it->clientId);
                cout << "Manager::processMailClientLogin:"
                     << " ClientId: " << it->clientId
                     << " ClientName: " << it->clientName << endl;
                addClientToDatabase(clientName);
            }
        }
    } else {
        Mail tempMail;
        tempMail.typeMail = CLIENT_LOGIN;
        memset(tempMail.data, 0 , sizeof (tempMail.data));
        sendMail(tempMail, mail.clientId);
        cout << "ERROR: Manager::processMailClientLogin: mail.clientId(" << mail.clientId
             << ") was found" << endl;
    }
}

//------------------------------------------------------------------------------------------
void Manager::processMailDisconnectServer(Mail& mail)
//------------------------------------------------------------------------------------------
{
    cout << "Manager::processMailDisconnectServer" << endl;
    mEvHndlr.responseDisconnectServer();
}

//------------------------------------------------------------------------------------------
void Manager::processMailDisconnectClient(Mail& mail)
//------------------------------------------------------------------------------------------
{
    Mail tempMail;
    tempMail.typeMail = MESSAGE;
    cout << "Manager::processMailDisconnectClient" << endl;
    for (auto it = mClients.begin(); it != mClients.end(); ++it) {
        if (0 == strcmp(it->clientName, mail.data)) {
            snprintf (tempMail.data,sizeof (tempMail.data), "[ ChatServer ]: %s disconnected you from the server\n"
                      , getClietName(mail.clientId));
            sendMail(tempMail, it->clientId);
            mEvHndlr.responseDisconnectClient(it->clientId);
            return;
        }
    }
    snprintf (tempMail.data,sizeof (tempMail.data), "[ ChatServer ]: clietn %s not found\n"
              , mail.data);
    sendMail(tempMail, mail.clientId);
}

//------------------------------------------------------------------------------------------
bool Manager::checkNewClientName(char* clientName)
//------------------------------------------------------------------------------------------
{
    for (auto it = mClients.begin(); it != mClients.end(); ++it) {
        if (0 == strcmp(it->clientName, clientName) ) {
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------------------
char* Manager::getClietName(int client)
//------------------------------------------------------------------------------------------
{
    for (auto it = mClients.begin(); it != mClients.end(); ++it) {
        if (it->clientId == client) {
            return it->clientName;
        }
    }
    cout << "ERORR:Manager::getClietName" << endl;
    return nullptr;
}

//------------------------------------------------------------------------------------------
void Manager::addClientToDatabase( char* clientName)
//------------------------------------------------------------------------------------------
{
    ofstream ofsDataBase = ofstream(FILE_NAME_FOR_DATABASE, std::ios::out | std::ios::app);
    if (ofsDataBase.is_open()) {
        ofsDataBase << clientName << "\n";
    }
}

//------------------------------------------------------------------------------------------
void Manager::parseClientLogin( char* clientName, char* clientPassword,Mail& mail)
//------------------------------------------------------------------------------------------
{
    int clientNameLen = 0;
    int clientPasswordLen = 0;

    // Ищем пробел и длинну строк
    char *space = strstr(mail.data, " ");
    clientNameLen = space - mail.data;
    clientPasswordLen = strlen(mail.data) - clientNameLen - 1;

    // Копируем первое слово
    strncpy(clientName, &mail.data[0], clientNameLen);
    clientName[clientNameLen] = '\0';

    // Копируем остальную строку
    strncpy(clientPassword, &space[1], clientPasswordLen + 1);

    cout << "Manager::parseClientLogin: clientName = " << clientName << " clientPassword = "
         << clientPassword << endl;
}




