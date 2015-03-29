#include "filelink_command.h"
#include "common_arguments.h"
#include "credentials.h"

#include <cmd/exceptions.h>
#include <lf/declarations.h>
#include <lf/engine.h>

namespace ui {

filelink_command::filelink_command(lf::engine& e)
    : cmd::command("filelink", "Uploads given file and creates filelink on it.")
    , m_engine(e)
    , m_expire_argument("expires", "<YYYY-MM-DD>", "Expire date for the filelink.")
    , m_file_argument("<file>", "File path to create filelink.")
{
    get_arguments().push_back(credentials::get_arguments());
    get_arguments().push_back(s_report_level_arg);
    get_arguments().push_back(m_expire_argument);
    get_arguments().push_back(m_file_argument);
}

void filelink_command::execute(const cmd::arguments& args)
{
    credentials c = credentials::manage(args);
    lf::report_level rl = s_report_level_arg.value(args);
    std::string expire = m_expire_argument.value(args);
    std::set<std::string> unnamed_args = m_file_argument.value(args);
    if (unnamed_args.size() != 1) {
        throw cmd::invalid_arguments("Need to specify only one file.");
    }
    m_engine.filelink(c.server(), c.api_key(), expire, *unnamed_args.begin(),
            rl, c.validate_flag());
}

}
