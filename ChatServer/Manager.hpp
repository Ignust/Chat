#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <queue>
using std::queue;
#include <list>
using std::list;

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
    void processMailType(Mail&);
    void processMailMessage(Mail&);
    void processMailClientLogin(Mail&);
    void processMailDisconnectServer(Mail&);
    bool checkNewClientName(Mail&);

private:
    list<Client> mClients;
    queue<Mail> mMails;
    EventHandler& mEvHndlr;
};

#endif // MANAGER_HPP
