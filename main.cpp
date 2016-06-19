#include <iostream>
#include <sstream>

#include "my_logger.h"
#include <sqlite3.h>


class CadDBException : public std::exception
{
public:
    CadDBException(const int code_, const char* msg_)
        : code(code_), msg(msg_)
    {
        ;
    }
    int code;
    std::string msg;
};


class CadDB
{
public:
    CadDB(const char* db_filename);
    ~CadDB();
    void CreateTestTable();
    void CreateCadTables();
private:
    sqlite3* m_db;
    std::string m_db_filename;
};

CadDB::CadDB(const char *db_filename)
    : m_db(NULL), m_db_filename(db_filename)
{
    int retval = sqlite3_open_v2(db_filename, &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (retval != SQLITE_OK)
    {
        std::stringstream msg;
        msg << "Can't open sqlite3 db: " << sqlite3_errmsg(m_db);
        sqlite3_close(m_db);
        m_db = NULL;
        throw CadDBException(-1, msg.str().c_str());
    }
    else
    {
        LOG(info) << "Opened sqlite db \"" << db_filename << "\".";
    }
}

CadDB::~CadDB()
{
    if (m_db != NULL)
    {
        sqlite3_close(m_db);
        LOG(info) << "Closed sqlite db \"" << m_db_filename << "\".";
    }
}

void CadDB::CreateTestTable()
{
    char* errmsg = 0;
    int retval = sqlite3_exec(m_db, "CREATE TABLE test(id PRIMARY KEY, col1, col2)", NULL, NULL, &errmsg);
    if (retval != SQLITE_OK)
    {
        std::stringstream msg;
        msg << "Error creating table: " << errmsg;
        throw CadDBException(-2, msg.str().c_str());
    }
    else
    {
        LOG(info) << "Created table 'test' in \"" << m_db_filename << "\"." ;
    }
}

void CadDB::CreateCadTables()
{
    char* errmsg = 0;
    int retval = 0;

    const char* createThermalModelsTable =
        "CREATE TABLE ThermalModels"
        "("
        "  ThermalModelID NONE NOT NULL,"
        "  Name NONE,"
        "  CONSTRAINT Key1 PRIMARY KEY (ThermalModelID)"
        ");";
    retval = sqlite3_exec(m_db, createThermalModelsTable, NULL, NULL, &errmsg);
    if (retval != SQLITE_OK)
    {
        std::stringstream msg;
        msg << "Error creating table: " << errmsg;
        throw CadDBException(-2, msg.str().c_str());
    }
    else
    {
        LOG(info) << "Created 'ThermalModels' table  in \"" << m_db_filename << "\"." ;
    }
}

int main(int, char*[])
{
    log_init();

    LOG(info) << "Starting CadDB test programme.";

    try
    {
        CadDB db("test.db");
        CadDB db1("test1.db");

        db.CreateTestTable();
        db1.CreateCadTables();
    }
    catch (CadDBException& exception)
    {
        LOG(fatal) << exception.msg << std::endl;
        return exception.code;
    }

    return 0;
}

