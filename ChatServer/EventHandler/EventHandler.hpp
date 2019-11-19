#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

class Server;
#include "Manager.hpp"
#include "IManagerListener.hpp"

class EventHandler: public IManagerListener
{
public:
    EventHandler();

    void start[[noreturn]]();

    //IManagerLister
    void responseDisconnectServer() override;
    void responseDisconnectClient(const int) override;

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
