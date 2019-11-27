#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <netinet/in.h>
#include <sys/select.h>
#include <string>

#define PORT_ADDR 12345
#define AMOUNT_EVENTS 5

struct Mail;

class Client
{
public:
    Client(int arraySzie, char* arrayLogin[]);
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
    void processingCommandExit();

    void processingInputCommand(const Mail&);
    void checkMessenger();
    void getmClientName();
    void getClientPassword();
    void sendClientLogin();
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
    std::string mClientName;
    std::string mClientPassword;
    bool mOutput;
};

#endif // CLIENT_HPP
