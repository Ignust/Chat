#ifndef MAILTYPES_HPP
#define MAILTYPES_HPP

#include <string>
#include <memory>

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
    char data[1024];
};

class CWrapMail
{
    public:
    CWrapMail(const Mail& mail,const int Id)
        : typeMail(mail.typeMail)
        , data(mail.data)
        , clientId(Id)
    {}

    void getMail( Mail& mail) const
    {
        std::copy(data.begin(), data.end(), mail.data);
        mail.typeMail = typeMail;
    }

    MAIL_TYPE typeMail;
    std::string data;
    const int clientId;
};

#endif // MAILTYPES_HPP


