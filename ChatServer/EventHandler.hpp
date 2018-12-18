#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <algorithm>

#include "Server.hpp"
#include "Manager.hpp"

class Manager;

class EventHandler
{
public:
    EventHandler();

    void start[[noreturn]]();
    void disconnectServer();

private:
    void subscribeClient();
    void unsubscribeClient(int);
    bool sendMail(int);

    void initReadSet();
    int getMaxClientId();
    void checkNewClient();
    void checkNewMails();

private:
    Server& mServer;
    Manager mManager;

    fd_set mReadset;
};

#endif // EVENTHANDLER_HPP
