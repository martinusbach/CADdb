#include "caddb.h"
#include "my_logger.h"
#include <string>
#include <fstream>
 
class ObjReader
{
public:
    ObjReader(const char *obj_filename);
    ~ObjReader();
    void saveToDB(CadDB& db);
private:
    std::string m_obj_filename;
};

ObjReader::ObjReader(const char* obj_filename)
    : m_obj_filename(obj_filename)
{
    ;
}

ObjReader::~ObjReader()
{
    ;
}

void ObjReader::saveToDB(CadDB &db)
{
    std::ifstream objFileStream(m_obj_filename, std::ifstream::in);
    if (!objFileStream.is_open())
    {
        LOG(fatal) << "Could not open obj file...";
        return; // TODO throw exception
    }

    const char coordChar[] = "XYZW";

    VertexInserter vi(db);

    const size_t max_chars = 256;
    char line[max_chars];
    char* token;
    while(objFileStream.getline(line, max_chars))
    {
        token = strtok(line, " ");
        if (token == NULL)
        {
            continue;
        }
        else if (*token == ' ' || *token == '#')
        {
            continue;
        }

        if (*token == 'v')
        {
            // start reading a vertex
            double coord;
            int i = 0;
            for (token = strtok(NULL, " ");
                 token != NULL && i < vi.m_nrInsertVals;
                 token = strtok(NULL, " "), ++i)
            {
                coord = strtod(token, NULL);
#ifdef DEBUG
                LOG(debug) << "Read " << coordChar[i] << " coordinate: " << coord;
#endif
                vi << coord;
            }
        }
        else if (*token == 'g')
        {
            // start reading a group
        }
        else if (*token == 'f')
        {
            // start reading a face
            int face_indices[3];
            int i = 0;
            for (token = strtok(NULL, " ");
                 token != NULL && i < 3;
                 token = strtok(NULL, " "), ++i)
            {
                face_indices[i] = strtol(token, NULL, 10);
            }
#ifdef DEBUG
            LOG(debug) << "face indices: " << face_indices[0] << "|" << face_indices[1] << "|" << face_indices[2] << "|";
#endif
        }
    }


}


int main(int, char*[])
{
    log_init();

    LOG(info) << "Starting CadDB test programme.";

    try
    {
        CadDB db("c:/temp/test.db");  // creates and opens an sqlite db
        db.CreateCadTables(); // creates the basic tables

//        ObjReader teapotReader("../sample_obj_files/teapot.obj");
//        teapotReader.saveToDB(db);

//        ObjReader humanoidReader("../sample_obj_files/humanoid_tri.obj");
//        humanoidReader.saveToDB(db);

        ObjReader minicooperReader("../sample_obj_files/mini_cooper.obj");
        minicooperReader.saveToDB(db);

    }
    catch (CadDBException& exception)
    {
        LOG(fatal) << exception.msg << std::endl;
        return exception.code;
    }
    catch (const boost::filesystem::filesystem_error& e)
    {
        LOG(fatal) << e.what();
        return -1;
    }
    catch (...)
    {
        LOG(fatal) << "Unhandled exception!";
        return -1;
    }

    return 0;
}

