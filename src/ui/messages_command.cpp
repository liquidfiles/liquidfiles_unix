#include "messages_command.h"
#include "common_arguments.h"
#include "credentials.h"

#include <cmd/exceptions.h>
#include <lf/declarations.h>
#include <lf/engine.h>

namespace ui {

messages_command::messages_command(lf::engine& e)
    : cmd::command("messages", "Lists the available messages.")
    , m_engine(e)
    , m_message_id_argument("message_id", "<id>", "Message id to show.")
    , m_sent_in_last_argument("sent_in_the_last", "<HOURS>", "Show messages sent in the last specified hours.")
    , m_sent_after_argument("sent_after", "YYYYMMDD", "Show messages sent after specified date.")
{
    arguments.push_back(credentials::get_arguments());
    arguments.push_back(s_report_level_arg);
    arguments.push_back(s_output_format_arg);
    arguments.push_back(m_message_id_argument);
    arguments.push_back(m_sent_in_last_argument);
    arguments.push_back(m_sent_after_argument);
}

void messages_command::execute(const cmd::arguments& args)
{
    credentials c = credentials::manage(args);
    std::string l = m_sent_in_last_argument.value(args);
    std::string f = m_sent_after_argument.value(args);
    std::string id = m_message_id_argument.value(args);
    lf::report_level rl = s_report_level_arg.value(args);
    lf::output_format of = s_output_format_arg.value(args);
    if (id == "") {
        m_engine.messages(c.server(), c.api_key(), l, f, of, rl, c.validate_flag());
    } else {
        m_engine.message(c.server(), c.api_key(), id, of, rl, c.validate_flag());
    }
}

}
