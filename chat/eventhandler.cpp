#include "eventhandler.hpp"
#include <sys/select.h>
#include <algorithm>
#include<iostream>
using namespace std;


//------------------------------------------------------------------------------------------
EventHandler::EventHandler()
    : mServer(Server::initServer())
    , mManager()
//------------------------------------------------------------------------------------------
{


}
void EventHandler::start()
//------------------------------------------------------------------------------------------
{
    while (1) {
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(mServer.getServer(), &readset);

        for( int i = mManager.getAmountOfClient(); i >= 0 ; i --){
            int client_Id = mManager.getClient(i);
            if(client_Id != -1){
                FD_SET(client_Id,&readset);
            }

        }

        //////
        timeval timeout;
        timeout.tv_sec = 15;
        timeout.tv_usec = 0;
        int maxClientId= mServer.getServer();
        for( int i = mManager.getAmountOfClient(); i >= 0 ; i --){
            int client_Id = mManager.getClient(i);
            if (client_Id > maxClientId){
                maxClientId = client_Id;
            }
        }
        //cout << "client_Id = " << b << endl;
        //int mx = max(mServer.getServer(),maxClientId );

        //int mx = max(mServer.getServer(), *max_element(mManager.getClients().begin(), mManager.getClients().end()));
        select(maxClientId+1, &readset, NULL, NULL,NULL );

        if(FD_ISSET(mServer.getServer(), &readset)){
            subscribeClient();
        }
        for( int i = mManager.getAmountOfClient(); i >= 0 ; i --){
            int client_Id = mManager.getClient(i);
            if(FD_ISSET(client_Id,&readset)){
                cout <<"get respons for " << client_Id << endl;
                sendMail(client_Id);
            }
        }
        mManager.processMails();
    }
}
//------------------------------------------------------------------------------------------
void EventHandler::subscribeClient()
//------------------------------------------------------------------------------------------
{
   mManager.pushClient(mServer.listenClients());
}
//------------------------------------------------------------------------------------------
void EventHandler::unsubscribeClient()
//------------------------------------------------------------------------------------------
{


}
//------------------------------------------------------------------------------------------
void EventHandler::sendMail(int mail)
//------------------------------------------------------------------------------------------
{
    mManager.pushMail(mail);
}

