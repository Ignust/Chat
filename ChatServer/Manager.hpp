#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <queue>
using std::queue;
#include <list>
using std::list;
#include<map>
using std::map;

#include "ClientTypes.hpp"
#include "MailTypes.hpp"
#include "DataBase.hpp"
#include"Tools.hpp"

//#include"EventHandler.hpp"
class EventHandler;

class Manager
{
public:
    Manager(EventHandler&);

    void pushClient(const int);
    void popClient(const int);
    bool pushMail(const int);
    void processMails();
    int getAmountOfClient() const;
    int getClient(int);

private:
    void sendMail(Mail&, int)const;
    void processMailType(const Mail&);
    void processMailMessage(const Mail&);
    void processMailClientLogin(const Mail&);
    void processInvalidPassword(const Mail&);
    void processClientFound(const Mail&, char*);
    int getClientId(const char*);
    bool checkClientNameInDatabase(const char*,const char*);
    void addClientToList(const Mail&, char*, char*);
    void processMailDisconnectServer(const Mail&);
    void processMailDisconnectClient(const Mail&);
    void DisconnectClient(const int);
    bool checkClientName(const char*);
    void addClientToListFromDatabase(const Mail, char*);
    char* getClietName(const int);
    void addClientToDatabase(const Client &);
    void parseClientLogin(char*, char*,const Mail&);
    void loadClientsFromTheDatabase();
    void parseClientFromTheDatabase(Client&, char*);

private:
    map<int,Client> mClients;
    queue<Mail> mMails;
    EventHandler& mEvHndlr;
    DataBase mDataBase;

};

#endif // MANAGER_HPP
