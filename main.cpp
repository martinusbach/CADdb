#include "caddb.h"
#include "my_logger.h"
#include <string>
#include <fstream>

#include <boost/filesystem.hpp>
namespace bfs = boost::filesystem;

#include <boost/program_options.hpp>
namespace bpo = boost::program_options;

class ObjReader
{
public:
    ObjReader(const std::string& obj_filename);
    ~ObjReader();
    void saveToDB(CadDB& db);
private:
    std::string m_obj_filename;
};

ObjReader::ObjReader(const std::string &obj_filename)
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

    const char coordChar[] = "xyzw";

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

class CmdLineExcp : public std::runtime_error
{
public:
    enum CmdLineExcpCode { BAD_ARGUMENT, SHOW_USAGE };
    CmdLineExcp(const std::string& msg, CmdLineExcpCode c)
        : std::runtime_error(msg)
        , code(c)
    {}
    CmdLineExcpCode code;
};

class CmdLineArgs
{
public:
    CmdLineArgs(int argc, char* argv[])
    {
        // Parse command line options
        bpo::options_description desc("Usage");
        desc.add_options()
            ("help", "list options")
            ("obj-file", bpo::value<std::string>(), "the wavefront obj file (input)")
            ("db-file", bpo::value<std::string>(), "the cad database (output) -- if not specified: "
                                                   "replace extension from <filename>.obj with .db")
        ;
        bpo::variables_map vm;
        bpo::store(bpo::parse_command_line(argc, argv, desc), vm);
        bpo::notify(vm);

        // List command line arguments
        if (vm.count("help"))
        {
            LOG(info) << "\n\n" << desc;
            throw CmdLineExcp("", CmdLineExcp::SHOW_USAGE);
        }
        else if (vm.count("obj-file") != 1 || vm.count("db-file") > 1)
        {
            LOG(info) << "\n\n" << desc;
            throw CmdLineExcp("Invalid command line argument(s).", CmdLineExcp::BAD_ARGUMENT);
        }

        // Determine input .obj file name
        objFileName = vm["obj-file"].as<std::string>();

        // Determine output .db file name
        if (vm.count("db-file") == 1)
        {
            dbFileName = vm["db-file"].as<std::string>();
        }
        else
        {
            bfs::path objFile(objFileName);
            dbFileName = "./" + objFile.stem().string() + ".db";
        }
    }


    // parsed cmd line options:

    std::string objFileName;
    std::string dbFileName;
};


int main(int argc, char* argv[])
{
    // Initialize loggers
    log_init();

    try
    {
        // Parse command line arugments
        CmdLineArgs cmdLineArgs(argc, argv);
        LOG(info) << "Starting CadDB programme; processing " << cmdLineArgs.objFileName << ".";
        LOG(info) << "Using cad database output file: " << cmdLineArgs.dbFileName << ".";

        // Main program
        CadDB db(cmdLineArgs.dbFileName);  // creates and opens an sqlite db
        db.CreateCadTables(); // creates the basic tables

        ObjReader objReader(cmdLineArgs.objFileName);
        objReader.saveToDB(db);
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
    catch (const CmdLineExcp& e)
    {
        if (e.code == CmdLineExcp::BAD_ARGUMENT)
        {
            LOG(fatal) << e.what();
            return -1;
        }
    }
    catch (...)
    {
        LOG(fatal) << "Unhandled exception!";
        return -1;
    }

    return 0;
}

