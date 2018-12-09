#ifndef MAIL_HPP
#define MAIL_HPP

enum MAIL_TYPE
{
    MESSAGE
    , COMMAND
};

struct Mail
{
    MAIL_TYPE typeMail;
    char data [1024];
    int clientId;
};

#endif // MAIL_HPP
