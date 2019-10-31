#ifndef MAILTYPES_HPP
#define MAILTYPES_HPP


enum MAIL_TYPE
{
    MESSAGE
    , COMMAND
    , CLIENT_LOGIN
    , DISCONNECT_SERVER
    , DISCONNECT_CLIENT
    , LAST_TYPE
};

struct Mail
{
    MAIL_TYPE typeMail;
    char data [1024];
    //int clientId;
};

#endif // MAILTYPES_HPP
