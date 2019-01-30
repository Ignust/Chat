#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <queue>
using std::queue;
#include <list>
using std::list;

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

    void pushClient(int);
    void popClient(int);
    bool pushMail(int);
    void processMails();
    int getAmountOfClient() const;
    int getClient(int);

private:
    void sendMail(Mail&, int);
    void processMailType(Mail&);
    void processMailMessage(Mail&);
    void processMailClientLogin(Mail&);
    void processInvalidPassword(Mail&);
    void processClientFound(Mail&, char*);
    int getClientId(char*);
    bool checkClientNameInDatabase(char*, char*);
    void addClientToList(Mail&, char*, char*);
    void processMailDisconnectServer(Mail&);
    void processMailDisconnectClient(Mail&);
    void DisconnectClient(int);
    bool checkNewClientName(char*);
    void addClientToListFromDatabase(Mail, char*);
    char* getClietName(int);
    void addClientToDatabase(const Client &);
    void parseClientLogin(char*, char*,Mail&);
    void loadClientsFromTheDatabase();
    void parseClientFromTheDatabase(Client&, char*);

private:
    list<Client> mClients;
    queue<Mail> mMails;
    EventHandler& mEvHndlr;
    DataBase mDataBase;

};

#endif // MANAGER_HPP
