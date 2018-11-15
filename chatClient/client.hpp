#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <netinet/in.h>
#include<sys/select.h>
#include"mail.hpp"

#define PORT_ADDR 12345

class Client
{
public:
    Client();
    void start();
    bool disconnectClient();


private:
    bool initClient();
    static void sendMessenger(int);
    static void checkMessenger(int);

private:

    int mSocket;
    struct sockaddr_in mSockAddr;
    fd_set mSet;
    Mail mMail;

};

#endif // CLIENT_HPP
