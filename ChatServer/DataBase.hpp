#ifndef DATABASE_HPP
#define DATABASE_HPP
#define FILE_NAME_FOR_DATABASE "DataBase.txt"

#include<fstream>
using std::fstream;




class DataBase
{
public:
    DataBase();
    ~DataBase();
    void writeToDatabase(char*);

private:
    fstream mfstream;


};

#endif // DATABASE_HPP
