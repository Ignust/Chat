#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <queue>
using std::queue;
#include <list>
using std::list;
#include <map>
using std::map;

#include "ClientTypes.hpp"
#include "MailTypes.hpp"
#include "DataBase.hpp"
#include "Tools.hpp"

class IManagerListener;

class Manager
{
public:
    Manager();

    void setListener(IManagerListener* listener);

    void pushClient(const int);
    void popClient(const int);
    bool pushMail(const int);
    void processMails();
    int getAmountOfClient() const;
    int getClient(int);

private:
    void sendMail(Mail&, int)const;
    void processMailType(const CWrapMail&);
    void processMailMessage(const CWrapMail&);
    void processMailClientLogin(const CWrapMail&);
    void processInvalidPassword(const CWrapMail&);
    void processClientFound(const CWrapMail&, char*);
    int getClientId(const char*);
    bool checkClientNameInDatabase(const char*,const char*);
    void addClientToList(const CWrapMail&, char*, char*);
    void processMailDisconnectServer(const CWrapMail&);
    void processMailDisconnectClient(const CWrapMail&);
    void DisconnectClient(const int);
    bool checkClientName(const char*);
    void addClientToListFromDatabase(const CWrapMail, char*);
    char* getClietName(const int);
    void addClientToDatabase(const Client &);
    void parseClientLogin(char*, char*,const CWrapMail&);
    void loadClientsFromTheDatabase();
    void parseClientFromTheDatabase(Client&, char*);

private:
    map<int,Client> mClients;
    queue<CWrapMail> mMails;
    IManagerListener* mListener;
    DataBase mDataBase;

};

#endif // MANAGER_HPP
