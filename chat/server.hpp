#ifndef SERVER_HPP
#define SERVER_HPP


class Server
{
public:

    static Server& initServer();
    void disconectClient(int);
    int listenClients();
    int getServer();


private:
    Server();
    ~Server();

private:
    int masterSocket;


};

#endif // SERVER_HPP
