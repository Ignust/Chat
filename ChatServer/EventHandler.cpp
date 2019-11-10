#include <iostream>
#include <algorithm>
#include <sys/select.h>

#include "EventHandler.hpp"
#include "Server.hpp"
//#include "Manager.hpp"

//-----------------------------------------------------------------------------
EventHandler::EventHandler()
    : mServer(Server::initServer())
    , mManager(*this)
    , mReadset()
//-----------------------------------------------------------------------------
{

}

//-----------------------------------------------------------------------------
void EventHandler::start[[noreturn]]()
//-----------------------------------------------------------------------------
{
    while (1) {
        initReadSet();
        checkNewClient();
        checkNewMails();
        mManager.processMails();
    }
}

//-----------------------------------------------------------------------------
void EventHandler::responseDisconnectServer()
//-----------------------------------------------------------------------------
{
    std::cout << "EventHandler::responseDisconnectServer" << std::endl;
    mServer.closeServer();
}

//-----------------------------------------------------------------------------
void EventHandler::responseDisconnectClient(int client)
//-----------------------------------------------------------------------------
{
    std::cout << "EventHandler::responseDisconnectClient" << std::endl;
    unsubscribeClient(client);
}

//-----------------------------------------------------------------------------
void EventHandler::subscribeClient()
//-----------------------------------------------------------------------------
{
   mManager.pushClient(mServer.listenClients());
}

//-----------------------------------------------------------------------------
void EventHandler::unsubscribeClient(const int client)
//-----------------------------------------------------------------------------
{
    std::cout << "EventHandler::unsubscribeClient: client = " << client << std::endl;
    mManager.popClient(client);
    mServer.disconectClient(client);
}

//-----------------------------------------------------------------------------
bool EventHandler::sendMail(const int mail)
//-----------------------------------------------------------------------------
{
    return  mManager.pushMail(mail);
}

//-----------------------------------------------------------------------------
void EventHandler::initReadSet()
//-----------------------------------------------------------------------------
{
    FD_ZERO(&mReadset);
    FD_SET(mServer.getServer(), &mReadset);

    for (int i = mManager.getAmountOfClient(); i >= 0 ; --i) {
        int client_Id = mManager.getClient(i);
        if (client_Id != -1) {
            FD_SET(client_Id,&mReadset);
        }
    }

    select(getMaxClientId()+1, &mReadset, nullptr, nullptr, nullptr);
}

//-----------------------------------------------------------------------------
int EventHandler::getMaxClientId()
//-----------------------------------------------------------------------------
{
    int maxClientId = mServer.getServer();

    for (int i = mManager.getAmountOfClient(); i >= 0 ; --i) {
        int client_Id = mManager.getClient(i);
        if (client_Id > maxClientId) {
            maxClientId = client_Id;
        }
    }

    return maxClientId;
}

//-----------------------------------------------------------------------------
void EventHandler::checkNewClient()
//-----------------------------------------------------------------------------
{
    if (FD_ISSET(mServer.getServer(), &mReadset)) {
        subscribeClient();
    }
}

//-----------------------------------------------------------------------------
void EventHandler::checkNewMails()
//-----------------------------------------------------------------------------
{
    for (int i = mManager.getAmountOfClient(); i >= 0 ; --i) {
        int client_Id = mManager.getClient(i);
        if (FD_ISSET(client_Id,&mReadset)) {
            std::cout << "EventHandler::checkNewMails: get respons for " << client_Id << std::endl;
            if (!(sendMail(client_Id))) {
                unsubscribeClient(client_Id);
            }
        }
    }
}
