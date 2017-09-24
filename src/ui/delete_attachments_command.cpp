#include "delete_attachments_command.h"
#include "common_arguments.h"
#include "credentials.h"

#include <cmd/exceptions.h>
#include <lf/declarations.h>
#include <lf/engine.h>

namespace ui {

delete_attachments_command::delete_attachments_command(lf::engine& e)
    : cmd::command{"delete_attachments", "Deletes the given attachments."}
    , m_engine{e}
    , m_message_id_argument{"message_id", "<id>", "Message id to delete attachments of it."}
    , m_attachment_ids_argument{"<id> ...", "Id(s) of attachments to delete."}
{
    arguments.push_back(credentials::get_arguments());
    arguments.push_back(s_report_level_arg);
    arguments.push_back(m_message_id_argument);
    arguments.push_back(m_attachment_ids_argument);
}

void delete_attachments_command::execute(const cmd::arguments& args)
{
    credentials c = credentials::manage(args);
    lf::report_level rl = s_report_level_arg.value(args);
    std::string id = m_message_id_argument.value(args);
    std::set<std::string> unnamed_args = m_attachment_ids_argument.value(args);
    if (!id.empty()) {
        m_engine.delete_attachments(c.server(), c.api_key(), id, rl, c.validate_flag());
    }
    m_engine.delete_attachments(c.server(), c.api_key(), unnamed_args, rl, c.validate_flag());
}

}
