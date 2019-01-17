#include "DataBase.hpp"

#include<iostream>
using std::cout;
using std::endl;

//-----------------------------------------------------------------------------
DataBase::DataBase()
    : mFstream()
    , readPosition(0)
    //-----------------------------------------------------------------------------
{
    mFstream.open(FILE_NAME_FOR_DATABASE, fstream::app);
    if (mFstream.tellg()== 0) {
        cout << "DataBase::DataBase: database is empty add superuser" << endl;
        mFstream << "superuser" << " " << "las123123" << " " << "4";
    }
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
    mFstream << "\n" << data ;
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
