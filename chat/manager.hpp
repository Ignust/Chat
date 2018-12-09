#ifndef MANAGER_HPP
#define MANAGER_HPP
#include"mail.hpp"
#include"client.h"

#include<set>
using std::set;
#include<queue>
using std::queue;
#include<list>
using std::list;


class Manager
{
public:
    Manager();

    void pushClient(int);
    void popClient(int);
    bool pushMail(int);
    void processMails();
    int getAmountOfClient();
    int getClient(int);

private:
    void processMailType(Mail&);
    void processMailMessage(Mail&);
    void processMailCommand(Mail&);



private:
    list<Client> mClients;
    queue<Mail> mMails;



};

#endif // MANAGER_HPP
