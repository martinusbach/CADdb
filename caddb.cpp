#include "caddb.h"
#include "my_logger.h"
#include <sstream>

namespace bfs = boost::filesystem;


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
//        try
//        {
            bfs::remove(dbFilename);
//        }
//        catch(const bfs::filesystem_error& e)
//        {
//            LOG()
//        }
        LOG(info) << "Removed existing db file: \"" << dbFilename << "\".";
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

void CadDB::runQuery(const char* query, const char* desc)
{
    char* errmsg = 0;
    int retval = sqlite3_exec(m_db, query, NULL, NULL, &errmsg);
    if (retval != SQLITE_OK)
    {
        std::stringstream msg;
        msg << "Error running SQL query with description \"" << desc << "\": " << errmsg;
        throw CadDBException(-2, msg.str().c_str());
    }
    else
    {
        LOG(info) << "Successfully ran query with description \"" << desc
                  << "\" on \"" << m_db_filename << "\"." ;
    }
}


void CadDB::CreateCadTables()
{
    const char* createThermModsTbl = QUOTE(
        CREATE TABLE ThermalModels
        (
          ThermalModelID NONE NOT NULL,
          Name NONE,
          CONSTRAINT Key1 PRIMARY KEY (ThermalModelID)
        );
    );
    runQuery(createThermModsTbl, "create table ThermalModels");

    const char* createVerticesTbl = QUOTE(
        CREATE TABLE Vertices
        (
          VertexID INTEGER NOT NULL,
          CoordX REAL,
          CoordY REAL,
          CoordZ REAL,
          CONSTRAINT Key4 PRIMARY KEY (VertexID)
        );
    );
    runQuery(createVerticesTbl, "create table Vertices");
}




VertexInserter::VertexInserter(CadDB& db)
    : m_iInsertVal(0)
    , m_insertVertexStmt(NULL)
{
    const char insertVertexQuery[] = QUOTE(
        INSERT INTO Vertices (CoordX, CoordY, CoordZ)
        VALUES (?, ?, ?)
    );
    int retVal = sqlite3_prepare_v2(db.m_db, insertVertexQuery, sizeof(insertVertexQuery), &m_insertVertexStmt, NULL);
    if (retVal != SQLITE_OK)
    {
        LOG(fatal) << "Problem in preparing the VertexInserter!";
        // TODO: throw exception
    }
}

VertexInserter::~VertexInserter()
{
    int retVal = sqlite3_finalize(m_insertVertexStmt);
    if (retVal != SQLITE_OK)
    {
        LOG(fatal) << "Problem in sqlite3_finalize()!";
    }
}

void VertexInserter::bind(double coord)
{
    m_iInsertVal++;
    int retVal = sqlite3_bind_double(m_insertVertexStmt, m_iInsertVal, coord);
    if (retVal != SQLITE_OK)
    {
        LOG(fatal) << "Problem in sqlite3_bind_double()!";
        // TODO: throw exception
    }
    if (m_iInsertVal == m_nrInsertVals)
    {
        retVal = sqlite3_step(m_insertVertexStmt);
        if (retVal == SQLITE_DONE)
        {
            LOG(debug) << "Successfully inserted vertex.";
        }
        else
        {
            LOG(fatal) << "Error inserting vertex!";
            // TODO: throw exception
        }
        retVal = sqlite3_reset(m_insertVertexStmt);
        if (retVal != SQLITE_OK)
        {
            LOG(fatal) << "Problem in sqlite3_reset()!";
            // TODO: throw exception
        }
        retVal = sqlite3_clear_bindings(m_insertVertexStmt);
        if (retVal != SQLITE_OK)
        {
            LOG(fatal) << "Problem in sqlite3_clear_bindings()!";
            // TODO: throw exception
        }
        m_iInsertVal = 0;
    }
}
