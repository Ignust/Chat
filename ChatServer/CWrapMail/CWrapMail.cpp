#include "CWrapMail.hpp"

#include<stdio.h>


//-----------------------------------------------------------------------------
CWrapMail::CWrapMail(const Mail& mail,const int Id)
    : typeMail(mail.typeMail)
    , data(mail.data)
    , clientId(Id)
//-----------------------------------------------------------------------------
{

}

//-----------------------------------------------------------------------------
void CWrapMail::getMail(Mail& mail)const
//-----------------------------------------------------------------------------
{
    std::copy(data.begin(), data.end(), mail.data);
    mail.typeMail = typeMail;
}
