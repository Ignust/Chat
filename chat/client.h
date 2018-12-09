#ifndef CLIENT_H
#define CLIENT_H


struct Client
{
    int clientId;
    char clientName[1024] = {0};
};



#endif // CLIENT_H
