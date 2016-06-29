#include "caddb.h"
#include "my_logger.h"
#include <sstream>
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

// This macro can be used to write multi-line string literals without having to add quotes.
#define QUOTE(...) #__VA_ARGS__


CadDB::CadDB(const char *dbFilename)
    : m_db(NULL), m_db_filename(dbFilename)
{
    createDatabase(dbFilename, true); // remove old db by default
}

CadDB::CadDB(const char *dbFilename, const bool removeOldFile)
    : m_db(NULL), m_db_filename(dbFilename)
{
    createDatabase(dbFilename, removeOldFile);
}

CadDB::~CadDB()
{
    if (m_db != NULL)
    {
        sqlite3_close(m_db);
        LOG(info) << "Closed sqlite db \"" << m_db_filename << "\".";
    }
}

void CadDB::createDatabase(const char *dbFilename, const bool removeOldFile)
{
    if (removeOldFile && bfs::exists(dbFilename))
    {
        LOG(info) << "Removed existing db file: \"" << dbFilename << "\".";
        bfs::remove(dbFilename);
    }

    int retval = sqlite3_open_v2(dbFilename, &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
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
        LOG(info) << "Opened sqlite db \"" << dbFilename << "\".";
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

    const char* createTableQuery = QUOTE(
        CREATE TABLE ThermalModels
        (
          ThermalModelID NONE NOT NULL,
          Name NONE,
          CONSTRAINT Key1 PRIMARY KEY (ThermalModelID)
        );
    );
    retval = sqlite3_exec(m_db, createTableQuery, NULL, NULL, &errmsg);
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


