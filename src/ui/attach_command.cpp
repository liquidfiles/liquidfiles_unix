#include "attach_command.h"
#include "credentials.h"
#include "common_arguments.h"

#include <cmd/exceptions.h>
#include <lf/declarations.h>
#include <lf/engine.h>

namespace ui {

attach_command::attach_command(lf::engine& e)
    : cmd::command("attach", "Uploads given files to server.")
    , m_engine(e)
    , m_files_argument("<file> ...", "File path(s) to upload.")
{
    get_arguments().push_back(credentials::get_arguments());
    get_arguments().push_back(s_report_level_arg);
    get_arguments().push_back(m_files_argument);
}

void attach_command::execute(const cmd::arguments& args)
{
    credentials c = credentials::manage(args);
    lf::report_level rl = s_report_level_arg.value(args);
    std::set<std::string> unnamed_args = m_files_argument.value(args);
    m_engine.attach(c.server(), c.api_key(), unnamed_args, rl, c.validate_flag());
}

}
