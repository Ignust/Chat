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
    void processMailType(const WrapMail&);
    void processMailMessage(const WrapMail&);
    void processMailClientLogin(const WrapMail&);
    void processInvalidPassword(const WrapMail&);
    void processClientFound(const WrapMail&, char*);
    int getClientId(const char*);
    bool checkClientNameInDatabase(const char*,const char*);
    void addClientToList(const WrapMail&, char*, char*);
    void processMailDisconnectServer(const WrapMail&);
    void processMailDisconnectClient(const WrapMail&);
    void DisconnectClient(const int);
    bool checkClientName(const char*);
    void addClientToListFromDatabase(const WrapMail, char*);
    char* getClietName(const int);
    void addClientToDatabase(const Client &);
    void parseClientLogin(char*, char*,const WrapMail&);
    void loadClientsFromTheDatabase();
    void parseClientFromTheDatabase(Client&, char*);

private:
    map<int,Client> mClients;
    queue<WrapMail> mMails;
    EventHandler& mEvHndlr;
    DataBase mDataBase;

};

#endif // MANAGER_HPP
