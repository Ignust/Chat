#ifndef MANAGER_HPP
#define MANAGER_HPP
#define FILE_NAME_FOR_DATABASE "DataBase.txt"

#include <queue>
using std::queue;
#include <list>
using std::list;
#include<fstream>
using std::ofstream;
using std::ifstream;

#include "ClientTypes.hpp"
#include "MailTypes.hpp"
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
    void processMailDisconnectServer(Mail&);
    void processMailDisconnectClient(Mail&);
    bool checkNewClientName(char*);
    char* getClietName(int);
    void addClientToDatabase(char*);
    void parseClientLogin(char*, char*,Mail&);

private:
    list<Client> mClients;
    queue<Mail> mMails;
    EventHandler& mEvHndlr;
    ofstream mDatabase;
};

#endif // MANAGER_HPP
