#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP
#include"server.hpp"
#include"manager.hpp"



class EventHandler
{
public:
    EventHandler();

    void start();
private:
    void subscribeClient();
    void unsubscribeClient();
    void sendMail(int);

private:
    Server& mServer;
    Manager mManager;
};

#endif // EVENTHANDLER_HPP
