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
    const char *welcomeMsg = "Welcome to the ChatServer";
    send(newClient, welcomeMsg, strlen(welcomeMsg) + 1, 0);
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
            mClients.erase(it);
            cout << " was erased";
        }
    }
    cout << endl;
}

//------------------------------------------------------------------------------------------
bool Manager::pushMail(int client)
//------------------------------------------------------------------------------------------
{
    Mail tempMail;
    int bytesRecv = static_cast<int>( recv(client, &tempMail, sizeof(Mail), 0) );
    if (bytesRecv <= 0) {
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
        processMailCommand(mail);
        break;
    default:
        cout << "ERROR: Manager::processMailType: mail.typeMail = "<< mail.typeMail << endl;
        break;
    }
}

//------------------------------------------------------------------------------------------
void Manager::processMailMessage(Mail& mail)
//------------------------------------------------------------------------------------------
{
    for (list<Client>::iterator it = mClients.begin(); it != mClients.end(); ++it) {
        if (it->clientId != mail.clientId /*&& it->clientName != NULL*/) {
            cout <<"Manager::processMailMessage: id = " << it->clientId <<"; data: " << mail.data << flush;
            send(it->clientId, mail.data, sizeof (mail.data), 0);
        }
    }
}

//------------------------------------------------------------------------------------------
void Manager::processMailCommand(Mail& mail)
//------------------------------------------------------------------------------------------
{
    if (checkNewClientName(mail)) {
        for (list<Client>::iterator it = mClients.begin(); it != mClients.end(); ++it) {
            if (it->clientId == mail.clientId) {
                strncpy(it->clientName, mail.data, sizeof (mail.data));
                cout << "Manager::processMailCommand:"
                     << " ClientId: " << it->clientId
                     << " ClientName: " << it->clientName << endl;
            }
        }
    } else {
        cout << "ERROR: Manager::processMailCommand: mail.clientId(" << mail.clientId << ") was found" << endl;
    }
}

//------------------------------------------------------------------------------------------
bool Manager::checkNewClientName(Mail& mail)
//------------------------------------------------------------------------------------------
{
    for (list<Client>::iterator it = mClients.begin(); it != mClients.end(); ++it) {
        if (0 == strcmp(it->clientName, mail.data) ) {
            return false;
        }
    }
    return true;
}

