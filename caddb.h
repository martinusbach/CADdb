#ifndef CADDB_H
#define CADDB_H

#include <string>
#include <sqlite3.h>

class CadDBException : public std::exception
{
public:
    CadDBException(const int code_, const char* msg_)
        : code(code_), msg(msg_)     {}
    int code;
    std::string msg;
};

class CadDB
{
public:
    CadDB(const char *dbFilename);
    CadDB(const char* dbFilename, const bool removeOldFile);
    ~CadDB();
    void CreateTestTable();
    void CreateCadTables();
private:
    void createDatabase(const char *dbFilename, const bool removeOldFile);
    sqlite3* m_db;
    std::string m_db_filename;
};

#endif // CADDB_H
