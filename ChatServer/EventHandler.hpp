#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

class Server;
#include "Manager.hpp"

class EventHandler
{
public:
    EventHandler();

    void start[[noreturn]]();
    void responseDisconnectServer();
    void responseDisconnectClient(const int);

private:
    void subscribeClient();
    void unsubscribeClient(const int);
    bool sendMail(const int);

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
