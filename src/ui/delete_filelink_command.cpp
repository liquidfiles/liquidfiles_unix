#include "delete_filelink_command.h"
#include "common_arguments.h"
#include "credentials.h"

#include <cmd/exceptions.h>
#include <lf/declarations.h>
#include <lf/engine.h>

namespace ui {

delete_filelink_command::delete_filelink_command(lf::engine& e)
    : cmd::command{"delete_filelink", "Deletes the given filelink."}
    , m_engine{e}
    , m_filelink_id_argument{"filelink_id", "<id>", "ID of filelink to delete."}
{
    arguments.push_back(credentials::get_arguments());
    arguments.push_back(s_report_level_arg);
    arguments.push_back(m_filelink_id_argument);
}

void delete_filelink_command::execute(const cmd::arguments& args)
{
    credentials c = credentials::manage(args);
    lf::report_level rl = s_report_level_arg.value(args);
    std::string id = m_filelink_id_argument.value(args);
    m_engine.delete_filelink(c.server(), c.api_key(), id,
            rl, c.validate_flag());
}

}
