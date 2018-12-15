#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <netinet/in.h>
#include<sys/select.h>
#include"MailTypes.hpp"

#define PORT_ADDR 12345
#define AMOUNT_EVENTS 5
#define ARRAY_SIZE 20

class Client
{
public:
    Client();
    void start[[noreturn]]();
    void disconnectClient();

private:
    bool initClient();
    void sendMessenger();
    void checkMessenger();
    char* getmClientName(char *);
    void sendClientName(char *);
    bool createSocket();
    bool connectSocket();
    void processMailType(Mail&);
    void processMailMessage(Mail&);
    void processMailCommand(Mail&);
    void processMailClientLOgin(Mail&);

private:

    int mSocket;
    struct sockaddr_in mSockAddr;
    fd_set mSet;
    Mail mMail;
    char mClientName[ARRAY_SIZE];

};

#endif // CLIENT_HPP
