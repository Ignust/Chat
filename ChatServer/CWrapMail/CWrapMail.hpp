#ifndef CWRAPMAIL_HPP
#define CWRAPMAIL_HPP

#include <string>

#include "MailTypes.hpp"

class CWrapMail
{
    public:

    CWrapMail(const Mail& mail,const int Id);
    void getMail( Mail& mail) const;


    MAIL_TYPE typeMail;
    std::string data;
    const int clientId;
};

#endif // CWRAPMAIL_HPP
