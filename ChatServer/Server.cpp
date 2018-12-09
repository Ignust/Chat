#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> //sockaddr_in
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
using std::cout;
using std::endl;

#include "Server.hpp"


//-----------------------------------------------------------------------------
Server& Server::initServer()
//---------------------------------------------const--------------------------------
{
    static Server mServer;
    return mServer;

}

//-----------------------------------------------------------------------------
Server::Server()
    : mMasterSocket(0)
//-----------------------------------------------------------------------------
{
    openSocket();
}

//-----------------------------------------------------------------------------
Server::~Server()
//-----------------------------------------------------------------------------
{
    closeSocket();
}

//-----------------------------------------------------------------------------
void Server::disconectClient(int fD)
//-----------------------------------------------------------------------------
{
    shutdown(fD, SHUT_RDWR);
    close(fD);
    cout << "Server::disconectClient: client: " << fD << " is disconnected" << endl;
}

//-----------------------------------------------------------------------------
int Server::listenClients()
//-----------------------------------------------------------------------------
{
    int slaveSocket = accept(mMasterSocket, nullptr, nullptr);
    if (slaveSocket < 0) {
        cout << "ERROR: Server::listenClients: slaveSocket(" << slaveSocket << ") < 0" << endl;
    } else {
        cout << "Server::listenClients: slaveSocket was accepted" << endl;
        fcntl(slaveSocket, F_SETFL, O_NONBLOCK);
    }

    return slaveSocket;
}

//-----------------------------------------------------------------------------
int Server::getServer() const
//-----------------------------------------------------------------------------
{
    return mMasterSocket;
}

//-----------------------------------------------------------------------------
void Server::openSocket()
//-----------------------------------------------------------------------------
{
    if (createSocket() && bindSocket() && listenSocket()) {
        cout << "Server::openSocket: mMasterSocket(" << mMasterSocket << ") was opened" << endl;
    }
}

//-----------------------------------------------------------------------------
void Server::closeSocket()
//-----------------------------------------------------------------------------
{
    close(mMasterSocket);
}

//-----------------------------------------------------------------------------
bool Server::createSocket()
//-----------------------------------------------------------------------------
{
    mMasterSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (mMasterSocket < 2) {
        cout << "ERROR: Server::createSocket: mMasterSocket = " << mMasterSocket << endl;
        return false;
    }

    fcntl(mMasterSocket, F_SETFL, O_NONBLOCK);
    return true;
}

//-----------------------------------------------------------------------------
bool Server::bindSocket()
//-----------------------------------------------------------------------------
{
    sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(12345);
    sockAddr.sin_addr.s_addr = htons(INADDR_ANY);

    if (bind(mMasterSocket, (sockaddr*)(&sockAddr), sizeof (sockAddr)) < 0) {
        cout << "ERROR: Server::bindSocket: bind < 0" << endl;
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool Server::listenSocket()
//-----------------------------------------------------------------------------
{
    if (listen(mMasterSocket,SOMAXCONN) < 0) {
        cout << "ERROR: Server::listenSocket: listen < 0" << endl;
        return false;
    }

    return true;
}
