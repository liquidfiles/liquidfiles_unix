#include "credentials.h"
#include "arguments.h"
#include "exceptions.h"

#include <io/messenger.h>

#include <cstdlib>
#include <errno.h>
#include <fstream>
#include <set>
#include <sys/stat.h>

namespace lf {

namespace {

std::string get_directory_path()
{
    std::string h = std::getenv("HOME");
    h += "/.liquidfiles/";
    return h;
}

}

void credentials::load(credentials& c)
{
    std::string d = get_directory_path();
    d += "/credentials";
    std::ifstream f(d.c_str());
    int version = 0;
    f >> version;
    switch (version) {
    case 1:
    {
        f >> c.m_server;
        f >> c.m_api_key;
        int x;
        f >> x;
        c.m_validate_flag = static_cast<validate_cert>(x);
    }
    default:
        ;
    }
}

void credentials::save(const credentials& c)
{
    std::string d = get_directory_path();
    struct stat sb;
    int x = stat(d.c_str(), &sb);
    if (x != 0 && errno == ENOTDIR) {
        io::mout << "Warning: could not save credentials." << io::endl;
        return;
    }
    if (x != 0) {
        mkdir(d.c_str(), S_IRWXU);
    }
    d += "/credentials";
    std::ofstream f(d.c_str());
    f << m_serial_version << std::endl;
    f << c.m_server << std::endl;
    f << c.m_api_key << std::endl;
    f << c.m_validate_flag << std::endl;
}

credentials credentials::manage(const arguments& args)
{
    credentials c;
    load(c);
    if (args.exists("--api_key")) {
        c.m_api_key = args["--api_key"];
    }
    if (args.exists("--server")) {
        c.m_server = args["--server"];
    }
    if (c.m_api_key == "") {
        throw missing_argument("--api_key");
    }
    if (c.m_server == "") {
        throw missing_argument("--server");
    }
    const std::set<std::string>& b = args.get_boolean_arguments();
    if (b.find("-k") != b.end()) {
        c.m_validate_flag = NOT_VALIDATE;
    }
    if (b.find("-s") != b.end()) {
        save(c);
    }
    return c;
}

}
