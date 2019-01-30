#include "DataBase.hpp"

#include<iostream>
using std::cout;
using std::endl;
#include<cstring>
#include<stdio.h>

//-----------------------------------------------------------------------------
DataBase::DataBase()
    : mFstream()
    , readPosition(0)
    //-----------------------------------------------------------------------------
{

    mFstream.open(FILE_NAME_FOR_DATABASE, fstream::app);
    if (mFstream.tellg()== 0) {
        cout << "DataBase::DataBase: database is empty add superuser" << endl;
        mFstream << "superuser" << " " << "las123123" << " " << "4\n";
    }
    cout << "DataBase::DataBase:----position:" << mFstream.tellg() << endl;
    mFstream.close();


}

//-----------------------------------------------------------------------------
DataBase::~DataBase()
//-----------------------------------------------------------------------------
{

}

//-----------------------------------------------------------------------------
void DataBase::writeToDatabase(char* data)
//-----------------------------------------------------------------------------
{
    mFstream.open(FILE_NAME_FOR_DATABASE, fstream::app);
    cout << "DataBase::writeToDatabase:---position:" << mFstream.tellg() << endl;
    mFstream  << data << "\n";
    mFstream.close();
}

//-----------------------------------------------------------------------------
bool DataBase::getClient(char* outLineClient)
//-----------------------------------------------------------------------------
{
    mFstream.open(FILE_NAME_FOR_DATABASE, fstream::in);
    if (!mFstream.is_open()) {
        return false;
    }

    mFstream.seekg(0, fstream::end);
    if (readPosition == mFstream.tellg()) {
        return false;
    }

    mFstream.seekg(readPosition, fstream::beg);
    mFstream.getline(outLineClient, 1024);
    readPosition = mFstream.tellg();
    mFstream.close();
    return true;
}

//-----------------------------------------------------------------------------
bool DataBase::getClientByName(char* outLineClient, char* name)
//-----------------------------------------------------------------------------
{
    char tempName[1024] = {};
    char tempPassword[1024] = {};
    long long endFile = 0;


    mFstream.open(FILE_NAME_FOR_DATABASE, fstream::in);
    if (!mFstream.is_open()) {
        cout << "Error DataBase::getClientByName:!mFstream.is_open()" << endl;
        mFstream.close();
        return false;
    }

    mFstream.seekg(0, fstream::end);
    endFile = mFstream.tellg();
    mFstream.seekg(0, fstream::beg);

    while (mFstream.tellg() != endFile) {
        //cout << "DataBase::checkClientInData:----position:" << mFstream.tellg() << endl;
        mFstream.getline(outLineClient, sizeof(char[1024]));// Error
        parseClientFromTheDatabase(tempName,tempPassword,outLineClient);
        if(0 == strcmp(name,tempName)) {
            mFstream.close();
            return true;
        }
        memset(tempName, 0, sizeof (tempName));
        memset(tempPassword, 0, sizeof (tempPassword));
    }
    mFstream.close();
    return false;
}


//-----------------------------------------------------------------------------
int DataBase::checkClientInData(char* clientName, char* clientPassword)
//-----------------------------------------------------------------------------
{
    char tempLine[1024] = {};
    char tempName[1024] = {};
    char tempPassword[1024] = {};
    long long endFile = 0;


    mFstream.open(FILE_NAME_FOR_DATABASE, fstream::in);
    if (!mFstream.is_open()) {
        cout << "Error DataBase::checkClientInData:!mFstream.is_open()" << endl;
        mFstream.close();
        return OTHER;
    }

    mFstream.seekg(0, fstream::end);
    endFile = mFstream.tellg();
    mFstream.seekg(0, fstream::beg);

    while (mFstream.tellg() != endFile) {
        //cout << "DataBase::checkClientInData:----position:" << mFstream.tellg() << endl;
        mFstream.getline(tempLine, sizeof(tempLine));
        parseClientFromTheDatabase(tempName,tempPassword,tempLine);
        if(0 == strcmp(clientName,tempName)) {
            if(0 == strcmp(clientPassword, tempPassword)){
                mFstream.close();
                return CLIENT_FOUND;
            }
            mFstream.close();
            return INVALID_PASSWORD;
        }
        memset(tempName, 0, sizeof (tempName));
        memset(tempPassword, 0, sizeof (tempPassword));
    }
    mFstream.close();
    return INVALID_NAME;
}

//-----------------------------------------------------------------------------
void DataBase::parseClientFromTheDatabase(char* clientName, char* clientPassword, char* data)
//-----------------------------------------------------------------------------
{
    int clientNameLen = 0;
    int clientPasswordLen = 0;

    // Ищем пробел и длинну строк
    char *space = strstr(data, " ");
    clientNameLen = space - data;
    clientPasswordLen = strlen(data) - clientNameLen - 1;

    // Копируем первое слово
    strncpy(clientName, &data[0], clientNameLen);
    clientName[clientNameLen] = '\0';

    // Копируем остальную строку
    strncpy(clientPassword, &space[1], clientPasswordLen -2);

    /*
    char *space2 = strstr(&space[1], " ");
    client.ClientLvl = *(space2+1);
    */

    cout << "DataBase::parseClientFromTheDatabase: clientName = " << clientName << " clientPassword = "
         << clientPassword << endl;

}

