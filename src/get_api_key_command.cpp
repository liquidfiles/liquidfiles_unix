#include "get_api_key_command.h"
#include "credentials.h"

#include <cmd/exceptions.h>
#include <lf/declarations.h>
#include <lf/engine.h>

namespace lf {

get_api_key_command::get_api_key_command(engine& e)
    : cmd::command("get_api_key",
            "[-k] --server=<url> --username=<email> --password=<password> [-s] [--report_level=<level>]",
            "Retrieves api key for the specified user.",
            "\t-k\n"
            "\t    If specified, do not validate server certificate.\n\n"
            "\t--server\n"
            "\t    The server URL.\n\n"
            "\t--username\n"
            "\t    Username.\n\n"
            "\t--password\n"
            "\t    Password.\n\n"
            "\t-s\n"
            "\t    If specified, saves current credentials in cache."
            " Credentials to save are - '-k', '--server' and retrieved key.\n\n"
            "\t--report_level\n"
            "\t    Level of reporting.\n"
            "\t    Valid values: silent, normal, verbose.\n"
            "\t    Default value: normal."
            )
    , m_engine(e)
{
}

void get_api_key_command::execute(const cmd::arguments& args)
{
    const std::string& server = args["--server"];
    if (server == "") {
        throw cmd::missing_argument("--server");
    }
    const std::string& user = args["--username"];
    if (user == "") {
        throw cmd::missing_argument("--username");
    }
    const std::string& password = args["--password"];
    if (password == "") {
        throw cmd::missing_argument("--password");
    }
    report_level rl = NORMAL;
    const std::string& rls = args["--report_level"];
    if (rls == "silent") {
        rl = SILENT;
    } else if (rls == "verbose") {
        rl = VERBOSE;
    } else if (rls != "" && rls != "normal") {
        throw cmd::invalid_argument_value("--report_level",
                "silent, normal, verbose");
    }
    const std::set<std::string>& b = args.get_boolean_arguments();
    validate_cert val = VALIDATE;
    if (b.find("-k") != b.end()) {
        val = NOT_VALIDATE;
    }
    const std::string& key = m_engine.get_api_key(server, user, password, rl, val);
    if (b.find("-s") != b.end()) {
        credentials c(server, key, val);
        credentials::save(c);
    }
}

}
