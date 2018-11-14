#include "server.hpp"

#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include <netinet/in.h> //sockaddr_in
#include<fcntl.h>
#include <unistd.h>
using namespace std;



//------------------------------------------------------------------------------------------
Server::Server()
    : masterSocket(0)
//------------------------------------------------------------------------------------------
{
    masterSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(masterSocket < 2){
        cout << "Erorr masterSocket" << endl;
        return;
    }else{
        cout << "masterSocket was created" << endl;
    }
    fcntl(masterSocket, F_SETFL, O_NONBLOCK);
    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(12345);
    sockAddr.sin_addr.s_addr = htons(INADDR_ANY);
    if(bind(masterSocket, (sockaddr*)(&sockAddr), sizeof (sockAddr)) < 0){
        cout << "Error bind(masterSocket)" << endl;
        return;
    }else{
        cout << "masterSocket was binded" << endl;
    }
    if(listen(masterSocket,SOMAXCONN) < 0){
        cout << "Erorr listen()" << endl;
        return;
    }else{
        cout << "masterSocket is listening" << endl;
    }



}
//------------------------------------------------------------------------------------------
Server& Server::initServer()
//------------------------------------------------------------------------------------------
{
    static Server mServer;
    return mServer;

}
//------------------------------------------------------------------------------------------
bool Server::disconectServer()
//------------------------------------------------------------------------------------------
{
    close(masterSocket);
}
//------------------------------------------------------------------------------------------
int Server::listenClients()
//------------------------------------------------------------------------------------------
{
    int slaveSocket = accept(masterSocket,0,0);
    if(slaveSocket < 0){
        cout << "Erorr accept()" << endl;
    }else{
        cout << "slaveSocket was accepted" << endl;
    }
    fcntl(slaveSocket, F_SETFL, O_NONBLOCK);
    return slaveSocket;

}
//------------------------------------------------------------------------------------------
int Server::getServer()
//------------------------------------------------------------------------------------------
{
    return masterSocket;
}
//------------------------------------------------------------------------------------------
Server::~Server()
//------------------------------------------------------------------------------------------
{

}
