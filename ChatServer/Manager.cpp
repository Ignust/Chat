#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include<stdio.h>//snprintf();
#include <iostream>
#include<memory>
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
void Manager::pushClient(const int newClient)
//------------------------------------------------------------------------------------------
{
    cout << __PRETTY_FUNCTION__ << " newClient = " << newClient << endl;
    mClients.emplace(newClient, Client{});
}

//------------------------------------------------------------------------------------------
void Manager::addClientToListFromDatabase(const WrapMail wrapMail, char* clientFromDatabase)
//------------------------------------------------------------------------------------------
{
    for(auto &it : mClients){
        if (it.first == wrapMail.clientId) {
            parseClientFromTheDatabase(it.second, clientFromDatabase);
            Mail tempMail;
            tempMail.typeMail = CLIENT_LOGIN;
            strcpy(tempMail.data,it.second.clientName);
            sendMail(tempMail,it.first);
        }
    }
}

//------------------------------------------------------------------------------------------
void Manager::popClient(const int ClientId)
//------------------------------------------------------------------------------------------
{
    cout << "Manager::popClient: ClientId = " << ClientId;
    for(auto &it : mClients){
        if (it.first == ClientId) {
            mClients.erase(it.first);
            cout << " was erased" << endl;
            return;
        }
    }
}

//------------------------------------------------------------------------------------------
bool Manager::pushMail(const int client)
//------------------------------------------------------------------------------------------
{
    Mail tempMail;
    int bytesRecv = static_cast<int>( recv(client, &tempMail, sizeof(Mail), 0) );
    if (bytesRecv <= 0) {
        cout << "Manager::pushMail: bytesRecv <= 0" << endl;
        DisconnectClient(client);
        return false;
    } else {
        //tempmail.mail.clientId = client;
        WrapMail tempWrapMail;
        tempWrapMail.mail = tempMail;
        tempWrapMail.clientId = client;
        mMails.push(tempWrapMail);
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
    for(auto it : mClients){
        if(number == 0){
            return it.first;
        }
        --number;
    }
    //cout << " Erorr: Manager::getClient(int) " << endl;
    return 0;
}

//------------------------------------------------------------------------------------------
void Manager::sendMail(Mail& meil, int clietn)const
//------------------------------------------------------------------------------------------
{
    cout << "Manager::sendMail: to -> " << clietn << endl;
    send(clietn, &meil, sizeof (Mail), 0);
}

//------------------------------------------------------------------------------------------
void Manager::processMailType(const WrapMail& wrapMail)
//------------------------------------------------------------------------------------------
{
    switch (wrapMail.mail.typeMail) {
    case MESSAGE:
        processMailMessage(wrapMail);
        break;
    case COMMAND:
        //
        break;
    case CLIENT_LOGIN:
        processMailClientLogin(wrapMail);
        break;
    case DISCONNECT_SERVER:
        processMailDisconnectServer(wrapMail);
        break;
    case DISCONNECT_CLIENT:
        processMailDisconnectClient(wrapMail);
        break;
    default:
        cout << "ERROR: Manager::processMailType: mail.mail.typeMail = "<< wrapMail.mail.typeMail << endl;
    }
}

//------------------------------------------------------------------------------------------
void Manager::processMailMessage(const WrapMail& wrapMail)
//------------------------------------------------------------------------------------------
{
    char* clientNameOfMail = getClietName(wrapMail.clientId);
    if(clientNameOfMail == nullptr){
        cout << "Manager::processMailMessage() clientNameOfMail == nullptr" << endl;
        return;
    }
    for(auto it : mClients){
        if (it.first != wrapMail.clientId
                && it.second.clientName[0] != 0
                && it.first !=0) {
            Mail tempMail;
            tempMail.typeMail = MESSAGE;
            snprintf (tempMail.data,sizeof (tempMail.data), "[ %.25s ]: %.992s",
                      clientNameOfMail, wrapMail.mail.data);
            cout <<"Manager::processMailMessage: id = " << it.first <<"; data: "
                << wrapMail.mail.data << flush;
            sendMail(tempMail, it.first);
        }
    }
}

//------------------------------------------------------------------------------------------
void Manager::processMailClientLogin(const WrapMail& wrapMail)
//------------------------------------------------------------------------------------------
{
    char clientName[1024] = {};
    char clientPassword[1024] = {};
    parseClientLogin(clientName, clientPassword,wrapMail);

    switch (mDataBase.checkClientInData(clientName,clientPassword)) {
    case INVALID_NAME:
        cout << "Manager::processMailClientLogin:processInvalidName()" << endl;
        //processInvalidName();
        addClientToList(wrapMail, clientName, clientPassword);
        break;
    case INVALID_PASSWORD:
        cout << "Manager::processMailClientLogin:processInvalidPassword()" << endl;
        processInvalidPassword(wrapMail);
        break;
    case CLIENT_FOUND:
        cout << "Manager::processMailClientLogin:processClientFound()" << endl;
        processClientFound(wrapMail, clientName);
        break;
    case OTHER:
        cout << "Manager::processMailClientLogin:OTHER()" << endl;
        break;
    default:
        cout << "Error Manager::processMailClientLogin: switch()" << endl;
        break;
    }
}

//------------------------------------------------------------------------------------------
void Manager::processInvalidPassword(const WrapMail& wrapMail)
//------------------------------------------------------------------------------------------
{
    Mail tempMail;
    tempMail.typeMail = CLIENT_LOGIN;
    snprintf (tempMail.data,sizeof (tempMail.data), "[ ChatServer ]: invalid password");
    sendMail(tempMail, wrapMail.clientId);
}

//------------------------------------------------------------------------------------------
void Manager::processClientFound(const WrapMail& wrapMail, char* clientName)
//------------------------------------------------------------------------------------------
{
    int clientId = getClientId(clientName);
    if (clientId > 0) {
        DisconnectClient(clientId);
    }

    char tempClientforDataBase[1024] = {};
    if (mDataBase.getClientByName(tempClientforDataBase,clientName)){
        addClientToListFromDatabase(wrapMail, tempClientforDataBase);
    }

}

//------------------------------------------------------------------------------------------
int Manager::getClientId(const char* clientName)
//------------------------------------------------------------------------------------------
{
    for (auto it = mClients.begin(); it != mClients.end(); ++it) {
        if (0 == strcmp(it->second.clientName, clientName) ) {
            return it->first;
        }
    }
    return -1;
}

bool Manager::checkClientNameInDatabase(const char* clientName,const char* clientPassword)
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
void Manager::addClientToList(const WrapMail& wrapMail, char* clientName, char* clientPassword)
//------------------------------------------------------------------------------------------
{
    for(auto &it : mClients){

        if (it.first == wrapMail.clientId) {
            strncpy(it.second.clientName, clientName, sizeof (char[1024]));
            strncpy(it.second.ClientPassword, clientPassword, sizeof (char[1024]));
            it.second.ClientLvl = '0';
            Mail tempMail;
            tempMail.typeMail = CLIENT_LOGIN;
            strcpy(tempMail.data,clientName);
            sendMail(tempMail,it.first);
            cout << "Manager::addClientToList:"
                 << " ClientId: " << it.first
                 << " ClientName: " << it.second.clientName << endl;
            addClientToDatabase(it.second);
        }
    }
}

//------------------------------------------------------------------------------------------
void Manager::processMailDisconnectServer(const WrapMail&)
//------------------------------------------------------------------------------------------
{
    cout << "Manager::processMailDisconnectServer" << endl;
    mEvHndlr.responseDisconnectServer();
}

//------------------------------------------------------------------------------------------
void Manager::processMailDisconnectClient(const WrapMail& wrapMail)
//------------------------------------------------------------------------------------------
{
    Mail tempMail;
    cout << "Manager::processMailDisconnectClient" << endl;
    for(auto &it : mClients){
        if (0 == strcmp(it.second.clientName, wrapMail.mail.data)) {
            tempMail.typeMail = DISCONNECT_CLIENT;
            snprintf (tempMail.data,sizeof (tempMail.data),
                      "[ ChatServer ]: %.100s disconnected you from the server\n"
                      , getClietName(wrapMail.clientId));
            sendMail(tempMail, it.first);
            mEvHndlr.responseDisconnectClient(it.first);
            return;
        }
    }
    tempMail.typeMail = MESSAGE;
    snprintf (tempMail.data,sizeof (tempMail.data), "[ ChatServer ]: client %.100s not found\n"
              , wrapMail.mail.data);
    sendMail(tempMail, wrapMail.clientId);
}

//------------------------------------------------------------------------------------------
void Manager::DisconnectClient(const int clientId)
//------------------------------------------------------------------------------------------
{
    Mail tempMail;
    tempMail.typeMail = DISCONNECT_CLIENT;
    cout << "Manager::DisconnectClient: " << clientId << endl;
    snprintf (tempMail.data,sizeof (tempMail.data), "[ ChatServer ]: you was disconnected from the server\n"
              );
    sendMail(tempMail, clientId);
    mEvHndlr.responseDisconnectClient(clientId);
}

//------------------------------------------------------------------------------------------
bool Manager::checkClientName(const char* clientName)
//------------------------------------------------------------------------------------------
{
    for (auto it = mClients.begin(); it != mClients.end(); ++it) {
        if (0 == strcmp(it->second.clientName, clientName) ) {
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------------------
char* Manager::getClietName(const int client)
//------------------------------------------------------------------------------------------
{
    for (auto it = mClients.begin(); it != mClients.end(); ++it) {
        if (it->first == client) {
            return it->second.clientName;
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
    snprintf(tempClientData, sizeof(tempClientData),"%.100s %.100s %c",
             client.clientName, client.ClientPassword, client.ClientLvl);
    mDataBase.writeToDatabase(tempClientData);
}

//------------------------------------------------------------------------------------------
void Manager::parseClientLogin( char* clientName, char* clientPassword,const WrapMail& wrapMail)
//------------------------------------------------------------------------------------------
{
    unsigned long clientNameLen = 0;
    unsigned long clientPasswordLen = 0;

    // Ищем пробел и длинну строк
    const char *space = strstr(wrapMail.mail.data, " ");
    clientNameLen =static_cast<unsigned long>(space - wrapMail.mail.data);
    clientPasswordLen = strlen(wrapMail.mail.data) - clientNameLen - 1;

    // Копируем первое слово
    strncpy(clientName, &wrapMail.mail.data[0], clientNameLen);
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
    while(mDataBase.(tempClient)) {
        pushClientFromDatabase(tempClient);
    }
    */
}

//------------------------------------------------------------------------------------------
void Manager::parseClientFromTheDatabase(Client& client, char* data)
//------------------------------------------------------------------------------------------
{
    unsigned long clientNameLen = 0;
    unsigned long clientPasswordLen = 0;

    // Ищем пробел и длинну строк
    char *space = strstr(data, " ");
    clientNameLen =static_cast<unsigned long>(space - data);
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



