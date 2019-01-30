#ifndef DATABASE_HPP
#define DATABASE_HPP
#define FILE_NAME_FOR_DATABASE "DataBase.txt"

//#include"ClientTypes.hpp"
#include"Tools.hpp"
#include<fstream>
using std::fstream;




class DataBase
{
public:
    DataBase();
    ~DataBase();
    void writeToDatabase(char*);
    bool getClient(char*);
    bool getClientByName(char*, char*);
    int checkClientInData(char*, char*);
    void parseClientFromTheDatabase(char*,char*,char*);

private:
    fstream mFstream;
    long long readPosition;
    //Client* mClient;


};

#endif // DATABASE_HPP
