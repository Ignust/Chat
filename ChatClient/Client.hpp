#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <netinet/in.h>
#include <sys/select.h>

#define PORT_ADDR 12345
#define AMOUNT_EVENTS 5
#define ARRAY_SIZE 20

struct Mail;

class Client
{
public:
    Client();
    void start[[noreturn]]();
    void disconnectClient();

private:
    bool initClient();
    void checkKeyboardInput();
    void sendMail(const Mail& );

    bool checkInputCommand(const Mail&);
    void processingCommandHelp();
    void processingCommandDisconnectServer();
    void processingCommandDisconnectClient();

    void processingInputCommand(const Mail&);
    void checkMessenger();
    char* getmClientName();
    void getClientPassword(char *);
    void sendClientLogin(char *);
    bool createSocket();
    bool connectSocket();
    void processMailType(const Mail&);
    void processMailMessage(const Mail&);
    void processMailCommand(const Mail&);
    void processMailClientLOgin(const Mail&);
    void processMailDisconnectClient(const Mail&);

private:
    int mSocket;
    fd_set mSet;
    char mClientName[ARRAY_SIZE];
    bool mOutput;
};

#endif // CLIENT_HPP
