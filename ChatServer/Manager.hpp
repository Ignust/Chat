#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <queue>
using std::queue;
#include <list>
using std::list;

#include "ClientTypes.hpp"
#include "MailTypes.hpp"

class Manager
{
public:
    Manager();

    void pushClient(int);
    void popClient(int);
    bool pushMail(int);
    void processMails();
    int getAmountOfClient() const;
    int getClient(int);

private:
    void processMailType(Mail&);
    void processMailMessage(Mail&);
    void processMailCommand(Mail&);
    bool checkNewClientName(Mail&);

private:
    list<Client> mClients;
    queue<Mail> mMails;
};

#endif // MANAGER_HPP
