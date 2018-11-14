#ifndef SERVER_HPP
#define SERVER_HPP


class Server
{
public:

    static Server& initServer();
    bool disconectServer();
    int listenClients();
    int getServer();


private:
    Server();
    ~Server();

private:
    int masterSocket;


};

#endif // SERVER_HPP
