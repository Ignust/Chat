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
    , mDataBase()
//------------------------------------------------------------------------------------------
{
    //loadClientsFromTheDatabase();
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
void Manager::addClientToListFromDatabase(Mail mail, char* clientFromDatabase)
//------------------------------------------------------------------------------------------
{
    for (auto it = mClients.begin(); it != mClients.end(); ++it) {
        if (it->clientId == mail.clientId) {
            parseClientFromTheDatabase(*it, clientFromDatabase);
            Mail tempMail;
            tempMail.typeMail = CLIENT_LOGIN;
            strcpy(tempMail.data,it->clientName);
            sendMail(tempMail,it->clientId);
        }
    }
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
    cout << "Manager::sendMail: to -> " << clietn << endl;
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
    for (auto it = mClients.begin(); it != mClients.end(); ++it) {
        if (it->clientId != mail.clientId
                && it->clientName[0] != 0
                && it->clientId !=0) {
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

    switch (mDataBase.checkClientInData(clientName,clientPassword)) {
    case INVALID_NAME:
        cout << "Manager::processMailClientLogin:processInvalidName()" << endl;
        //processInvalidName();
        addClientToList(mail, clientName, clientPassword);
        break;
    case INVALID_PASSWORD:
        cout << "Manager::processMailClientLogin:processInvalidPassword()" << endl;
        processInvalidPassword(mail);
        break;
    case CLIENT_FOUND:
        cout << "Manager::processMailClientLogin:processClientFound()" << endl;
        processClientFound(mail, clientName);
        break;
    case OTHER:
        cout << "Manager::processMailClientLogin:OTHER()" << endl;
        break;
    default:
        cout << "Error Manager::processMailClientLogin: switch()" << endl;
        break;
    }
    /*
    if (checkNewClientName(clientName)) {
        if(checkClientNameInDatabase(clientName, clientPassword)) {
            addClientToList(mail, clientName, clientPassword);
        }
    } else {
        Mail tempMail;
        tempMail.typeMail = CLIENT_LOGIN;
        memset(tempMail.data, 0 , sizeof (tempMail.data));
        sendMail(tempMail, mail.clientId);
        cout << "ERROR: Manager::processMailClientLogin: mail.clientId(" << mail.clientId
             << ") was found" << endl;
    }
    */
}

//------------------------------------------------------------------------------------------
void Manager::processInvalidPassword(Mail& mail)
//------------------------------------------------------------------------------------------
{
    Mail tempMail;
    tempMail.typeMail = CLIENT_LOGIN;
    snprintf (tempMail.data,sizeof (tempMail.data), "[ ChatServer ]: invalid password");
    sendMail(tempMail, mail.clientId);
}

//------------------------------------------------------------------------------------------
void Manager::processClientFound(Mail& mail, char* clientName)
//------------------------------------------------------------------------------------------
{
    int clientId = getClientId(clientName);
    if (clientId > 0) {
        DisconnectClient(clientId);
    }

    char tempClientforDataBase[1024] = {};
    if (mDataBase.getClientByName(tempClientforDataBase,clientName)){
        addClientToListFromDatabase(mail, tempClientforDataBase);
    }

}

//------------------------------------------------------------------------------------------
int Manager::getClientId(char* clientName)
//------------------------------------------------------------------------------------------
{
    for (auto it = mClients.begin(); it != mClients.end(); ++it) {
        if (0 == strcmp(it->clientName, clientName) ) {
            return it->clientId;
        }
    }
    return -1;
}

bool Manager::checkClientNameInDatabase(char* clientName, char* clientPassword)
//------------------------------------------------------------------------------------------
{
    cout << "Manager::checkClientNameInDatabase()" << endl;
    Client tempClient;
    char ClientFromDatabase[1024] = {};
    while(mDataBase.getClient(ClientFromDatabase)) {
        parseClientFromTheDatabase(tempClient, ClientFromDatabase);
        if (0 == strcmp(clientName, tempClient.clientName) ) {
            if (0 == strcmp(clientPassword, tempClient.ClientPassword) ) {

            }
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------------------
void Manager::addClientToList(Mail& mail, char* clientName, char* clientPassword)
//------------------------------------------------------------------------------------------
{
    for (auto it = mClients.begin(); it != mClients.end(); ++it) {

        if (it->clientId == mail.clientId) {
            strncpy(it->clientName, clientName, sizeof (char[1024]));
            strncpy(it->ClientPassword, clientPassword, sizeof (char[1024]));
            it->ClientLvl = '0';
            Mail tempMail;
            tempMail.typeMail = CLIENT_LOGIN;
            strcpy(tempMail.data,clientName);
            sendMail(tempMail,it->clientId);
            cout << "Manager::addClientToList:"
                 << " ClientId: " << it->clientId
                 << " ClientName: " << it->clientName << endl;
            addClientToDatabase(*it);
        }

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
void Manager::DisconnectClient(int clientId)
//------------------------------------------------------------------------------------------
{
    Mail tempMail;
    tempMail.typeMail = MESSAGE;
    cout << "Manager::DisconnectClient: " << clientId << endl;
    snprintf (tempMail.data,sizeof (tempMail.data), "[ ChatServer ]: you was disconnected from the server\n"
              );
    sendMail(tempMail, clientId);
    mEvHndlr.responseDisconnectClient(clientId);
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
void Manager::addClientToDatabase(const Client& client)
//------------------------------------------------------------------------------------------
{
    char tempClientData[1024] = {};
    snprintf(tempClientData, sizeof(tempClientData),"%s %s %c", client.clientName, client.ClientPassword
             , client.ClientLvl);
    mDataBase.writeToDatabase(tempClientData);
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

//------------------------------------------------------------------------------------------
void Manager::loadClientsFromTheDatabase()
//------------------------------------------------------------------------------------------
{
    cout << "Error Manager::loadClientsFromTheDatabase()" << endl;
    /*
    cout << "Manager::loadClientsFromTheDatabase()" << endl;
    char tempClient[1024] = {};
    while(mDataBase.getClient(tempClient)) {
        pushClientFromDatabase(tempClient);
    }
    */
}

//------------------------------------------------------------------------------------------
void Manager::parseClientFromTheDatabase(Client& client, char* data)
//------------------------------------------------------------------------------------------
{
    int clientNameLen = 0;
    int clientPasswordLen = 0;

    // Ищем пробел и длинну строк
    char *space = strstr(data, " ");
    clientNameLen = space - data;
    clientPasswordLen = strlen(data) - clientNameLen - 1;

    // Копируем первое слово
    strncpy(client.clientName, &data[0], clientNameLen);
    client.clientName[clientNameLen] = '\0';

    // Копируем остальную строку
    strncpy(client.ClientPassword, &space[1], clientPasswordLen -2);

    char *space2 = strstr(&space[1], " ");
    client.ClientLvl = *(space2+1);

    cout << "Manager::parseClientFromTheDatabase: clientName = " << client.clientName << " clientPassword = "
         << client.ClientPassword << " clientLvl = "<< client.ClientLvl << endl;

}



