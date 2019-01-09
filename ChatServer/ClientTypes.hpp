#ifndef CLIENTTYPES_HPP
#define CLIENTTYPES_HPP


struct Client
{
    int clientId;
    char clientName[1024];
    char ClientPassword[1024];
    int ClientLvl ;
};



#endif // CLIENTTYPES_HPP
