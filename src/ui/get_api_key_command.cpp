#include "get_api_key_command.h"
#include "common_arguments.h"
#include "credentials.h"

#include <cmd/exceptions.h>
#include <lf/declarations.h>
#include <lf/engine.h>

namespace ui {

get_api_key_command::get_api_key_command(lf::engine& e)
    : cmd::command("get_api_key", "Retrieves api key for the specified user.")
    , m_engine(e)
    , m_validate_cert_argument("k", "If specified, do not validate server certificate.")
    , m_server_argument("server", "<url>", "The server URL.")
    , m_username_argument("username", "<email>", "Username.")
    , m_password_argument("password", "<password>", "Password.")
    , m_save_argument("s", "If specified, saves current credentials in cache."
            " Credentials to save are - '-k', '--server' and retrieved key.")
{
    get_arguments().push_back(m_validate_cert_argument);
    get_arguments().push_back(m_server_argument);
    get_arguments().push_back(m_username_argument);
    get_arguments().push_back(m_password_argument);
    get_arguments().push_back(m_save_argument);
    get_arguments().push_back(s_report_level_arg);
}

void get_api_key_command::execute(const cmd::arguments& args)
{
    std::string server = m_server_argument.value(args);
    std::string user = m_username_argument.value(args);
    std::string password = m_password_argument.value(args);
    lf::report_level rl = s_report_level_arg.value(args);
    lf::validate_cert val = m_validate_cert_argument.value(args);
    const std::string& key = m_engine.get_api_key(server, user, password, rl, val);
    bool s = m_save_argument.value(args);
    if (s) {
        credentials c(server, key, val);
        credentials::save(c);
    }
}

}
