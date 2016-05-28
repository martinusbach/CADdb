#include <iostream>

#include "my_logger.h"
#include <sqlite3.h>

int main(int, char*[])
{
    std::cout << "Hello World!" << std::endl;

    log_init();

    LOG(trace)   << "A trace severity message";
    LOG(debug)   << "A debug severity message";
    LOG(info)    << "An informational severity message";
    LOG(warning) << "A warning severity message: " << 1 << ", " << 1.0/3.0;
    LOG(error)   << "An error severity message";
    LOG(fatal)   << "A fatal severity message";

    sqlite3* db;

    // open db
    int retval = sqlite3_open("test.db", &db);
    if (retval != 0)
    {
        LOG(fatal) << "Can't open sqlite3 db: " << sqlite3_errmsg(db);
        sqlite3_close(db);
        return 1;
    }
    else
    {
        LOG(info) << "Opened sqlite db 'test.db'";
    }

    // create a table
    char* errmsg = 0;
    retval = sqlite3_exec(db, "CREATE TABLE test(id PRIMARY KEY, col1, col2)", NULL, NULL, &errmsg);
    if (retval != 0)
    {
        LOG(fatal) << "Error creating table: " << errmsg;
        sqlite3_close(db);
        return 2;
    }
    else
    {
        LOG(info) << "Created table 'test'";
    }

    sqlite3_close(db);
    LOG(info) << "Closed sqlite db 'test.db'";
    return 0;
}

