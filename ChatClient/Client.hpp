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
    void checkKeyboardInput();
    void sendMail(Mail& );

    bool checkInputCommand(Mail&);
    void processingCommandHelp();
    void processingCommandDisconnectServer();
    void processingCommandDisconnectClient();

    void processingInputCommand(Mail&);
    void checkMessenger();
    char* getmClientName(char *);
    void getClientPassword(char *);
    void sendClientLogin(char *);
    bool createSocket();
    bool connectSocket();
    void processMailType(Mail&);
    void processMailMessage(Mail&);
    void processMailCommand(Mail&);
    void processMailClientLOgin(Mail&);
    void processMailDisconnectClient(Mail&);

private:

    int mSocket;
    struct sockaddr_in mSockAddr;
    fd_set mSet;
    Mail mMail;
    char mClientName[ARRAY_SIZE];
    bool mOutput;

};

#endif // CLIENT_HPP
