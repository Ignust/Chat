#ifndef MAILTYPES_HPP
#define MAILTYPES_HPP

enum MAIL_TYPE
{
    MESSAGE
    , COMMAND
};

struct Mail
{
    MAIL_TYPE typeMail;
    char data[1024];
    int clientId;
};

#endif // MAILTYPES_HPP
