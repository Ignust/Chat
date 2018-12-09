#ifndef SERVER_HPP
#define SERVER_HPP

class Server
{
public:
    static Server& initServer();
    void disconectClient(int);
    int listenClients();
    int getServer() const;

private:
    Server();
    ~Server();

    void openSocket();
    void closeSocket();

    bool createSocket();
    bool bindSocket();
    bool listenSocket();

private:
    int mMasterSocket;
};

#endif // SERVER_HPP
