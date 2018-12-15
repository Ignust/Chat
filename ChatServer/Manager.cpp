#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <iostream>
using std::cout;
using std::endl;
using std::flush;

#include "Manager.hpp"

//------------------------------------------------------------------------------------------
Manager::Manager()
    : mClients()
    , mMails()
//------------------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------------------
void Manager::pushClient(int newClient)
//------------------------------------------------------------------------------------------
{
    //const char *welcomeMsg = "Welcome to the ChatServer";
    //send(newClient, welcomeMsg, strlen(welcomeMsg) + 1, 0);
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
    //cout << "43"<< endl;
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

        break;
    case CLIENT_LOGIN:
        processMailClientLogin(mail);
        break;
    default:
        cout << "ERROR: Manager::processMailType: mail.typeMail = "<< mail.typeMail << endl;
    }
}

//------------------------------------------------------------------------------------------
void Manager::processMailMessage(Mail& mail)
//------------------------------------------------------------------------------------------
{
    for (auto it = mClients.begin(); it != mClients.end(); ++it) {
        if (it->clientId != mail.clientId && it->clientName[0] != 0) {
            cout <<"Manager::processMailMessage: id = " << it->clientId <<"; data: " << mail.data << flush;
            send(it->clientId, &mail, sizeof (Mail), 0);
        }
    }
}

//------------------------------------------------------------------------------------------
void Manager::processMailClientLogin(Mail& mail)
//------------------------------------------------------------------------------------------
{
    if (checkNewClientName(mail)) {
        for (auto it = mClients.begin(); it != mClients.end(); ++it) {
            if (it->clientId == mail.clientId) {
                strncpy(it->clientName, mail.data, sizeof (mail.data));
                Mail tempMail;
                tempMail.typeMail = CLIENT_LOGIN;
                strcpy(tempMail.data, it->clientName);
                send(it->clientId, &tempMail, sizeof (Mail),0);
                cout << "Manager::processMailClientLogin:"
                     << " ClientId: " << it->clientId
                     << " ClientName: " << it->clientName << endl;
            }
        }
    } else {
        Mail tempMail;
        tempMail.typeMail = CLIENT_LOGIN;
        memset(tempMail.data, 0 , sizeof (tempMail.data));
        send(mail.clientId, &tempMail, sizeof (Mail),0);
        cout << "ERROR: Manager::processMailClientLogin: mail.clientId(" << mail.clientId << ") was found" << endl;
    }
}

//------------------------------------------------------------------------------------------
bool Manager::checkNewClientName(Mail& mail)
//------------------------------------------------------------------------------------------
{
    for (auto it = mClients.begin(); it != mClients.end(); ++it) {
        if (0 == strcmp(it->clientName, mail.data) ) {
            return false;
        }
    }
    return true;
}

