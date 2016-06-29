#include "caddb.h"
#include "my_logger.h"


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

