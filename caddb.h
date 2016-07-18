#ifndef CADDB_H
#define CADDB_H

#include <string>
#include <sqlite3.h>
#include <boost/filesystem.hpp>


// This macro can be used to write multi-line string literals without having to add quotes.
#define QUOTE(...) #__VA_ARGS__


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
    friend class VertexInserter;
public:
    CadDB(const std::string &dbFilename);
    CadDB(const std::string &dbFilename, const bool removeOldFile);
    ~CadDB();
    void CadDB::runQuery(const char *query, const char *desc);
    void CreateCadTables();
private:
    void createDatabase(const std::string &dbFilename, const bool removeOldFile);
    sqlite3* m_db;
    std::string m_db_filename;
};


class VertexInserter
{
public:
    VertexInserter(CadDB &db);
    ~VertexInserter();
    VertexInserter& VertexInserter::operator<<(const double coord);
    const int m_nrInsertVals = 3;
private:
    sqlite3_stmt* m_insertVertexStmt;
    int m_iInsertVal;
    CadDB* m_db_p;
};

#endif // CADDB_H
