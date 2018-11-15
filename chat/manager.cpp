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
    cout << "newClient = " << newClient << endl;
    mClients.insert(newClient);
}
//------------------------------------------------------------------------------------------
void Manager::pushMail(int client)
//------------------------------------------------------------------------------------------
{
    Mail tempMail;
    recv(client,&tempMail,sizeof (Mail),0);
    tempMail.clientId = client;
    mMails.push(tempMail);
}
//------------------------------------------------------------------------------------------
void Manager::processMails()
//------------------------------------------------------------------------------------------
{
    if(!mMails.empty()){
        processMailType(mMails.front());
        mMails.pop();
    }else{
        cout <<"Mail empty" << endl;
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
            cout <<"send mail : " << mail.data << "from: " << *it << endl;
            send(*it, mail.data, sizeof (mail.data), 0);
        }
    }
}
//------------------------------------------------------------------------------------------
void Manager::processMailCommand(Mail& mail)
//------------------------------------------------------------------------------------------
{

}


