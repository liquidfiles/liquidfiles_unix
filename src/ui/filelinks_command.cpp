#include "filelinks_command.h"
#include "common_arguments.h"
#include "credentials.h"

#include <cmd/exceptions.h>
#include <lf/declarations.h>
#include <lf/engine.h>

namespace ui {

filelinks_command::filelinks_command(lf::engine& e)
    : cmd::command("filelinks", "Lists the available filelinks.")
    , m_engine(e)
    , m_limit_argument("limit", "<number>", "Limit of filelinks list.")
{
    arguments.push_back(credentials::get_arguments());
    arguments.push_back(s_report_level_arg);
    arguments.push_back(s_output_format_arg);
    arguments.push_back(m_limit_argument);
}

void filelinks_command::execute(const cmd::arguments& args)
{
    credentials c = credentials::manage(args);
    lf::report_level rl = s_report_level_arg.value(args);
    lf::output_format of = s_output_format_arg.value(args);
    std::string limit = m_limit_argument.value(args);
    m_engine.filelinks(c.server(), c.api_key(), limit, of, rl, c.validate_flag());
}

}
