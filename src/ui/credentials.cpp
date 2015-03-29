#include "credentials.h"

#include <cmd/arguments.h>
#include <cmd/exceptions.h>
#include <io/messenger.h>

#include <cstdlib>
#include <errno.h>
#include <fstream>
#include <set>
#include <sys/stat.h>

namespace ui {

cmd::argument_definition<std::string, cmd::NAMED_ARGUMENT, false>
    credentials::m_server_arg("server", "<url>", "The server URL."
            " If not specified, tries to retrieve from saved credentials.");

cmd::argument_definition<std::string, cmd::NAMED_ARGUMENT, false>
    credentials::m_api_key_arg("api_key", "<key>", "API key of liquidfiles, to login to system."
            " If not specified, tries to retrieve from saved credentials.");

cmd::argument_definition<lf::validate_cert, cmd::BOOLEAN_ARGUMENT, false>
    credentials::m_validate_cert_arg("k", "If specified, do not validate server certificate."
            " If not specified, tries to retrieve from saved credentials.");

cmd::argument_definition<bool, cmd::BOOLEAN_ARGUMENT, false>
    credentials::m_save_arg("s", "If specified, saves current credentials in cache."
            " Credentials to save are - '-k', '--server' and '--api_key'.");

cmd::argument_definition_container credentials::m_arguments;

namespace {

std::string get_directory_path()
{
    std::string h = std::getenv("HOME");
    h += "/.liquidfiles/";
    return h;
}

}

void credentials::init()
{
    m_arguments.push_back(m_server_arg);
    m_arguments.push_back(m_api_key_arg);
    m_arguments.push_back(m_validate_cert_arg);
    m_arguments.push_back(m_save_arg);
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
        c.m_validate_flag = static_cast<lf::validate_cert>(x);
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

credentials credentials::manage(const cmd::arguments& args)
{
    credentials c;
    load(c);
    std::string v = m_api_key_arg.value(args);
    if (!v.empty()) {
        c.m_api_key = v;
    }
    v = m_server_arg.value(args);
    if (!v.empty()) {
        c.m_server = v;
    }
    if (c.m_api_key == "") {
        throw cmd::missing_argument(m_api_key_arg.name());
    }
    if (c.m_server == "") {
        throw cmd::missing_argument(m_server_arg.name());
    }
    lf::validate_cert vv = m_validate_cert_arg.value(args);
    if (vv == lf::NOT_VALIDATE) {
        c.m_validate_flag = vv;
    }
    bool s = m_save_arg.value(args);
    if (s) {
        save(c);
    }
    return c;
}

}
