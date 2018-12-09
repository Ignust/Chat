#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <netinet/in.h>
#include<sys/select.h>
#include"mail.hpp"

#define PORT_ADDR 12345
#define AMOUNT_EVENTS 5
#define ARRAY_SIZE 20

class Client
{
public:
    Client();
    void start();
    bool disconnectClient();


private:
    bool initClient();
    void sendMessenger();
    void checkMessenger();
    void getClientName();
    void sendClientName();

private:

    int mSocket;
    struct sockaddr_in mSockAddr;
    fd_set mSet;
    Mail mMail;
    char clientName[ARRAY_SIZE];

};

#endif // CLIENT_HPP
