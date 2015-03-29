#include "file_request_command.h"
#include "common_arguments.h"
#include "credentials.h"

#include <cmd/exceptions.h>
#include <lf/declarations.h>
#include <lf/engine.h>

namespace ui {

file_request_command::file_request_command(lf::engine& e)
    : cmd::command("file_request", "Sends the file request to specified user.")
    , m_engine(e)
    , m_to_argument("to", "<username>", "User name or email, to send file request.")
    , m_message_argument("message", "<string>", "Message text of composed email.", "")
    , m_subject_argument("subject", "<string>", "Subject of composed email.", "")
{
    get_arguments().push_back(credentials::get_arguments());
    get_arguments().push_back(s_report_level_arg);
    get_arguments().push_back(m_to_argument);
    get_arguments().push_back(m_subject_argument);
    get_arguments().push_back(m_message_argument);
}

void file_request_command::execute(const cmd::arguments& args)
{
    credentials c = credentials::manage(args);
    std::string user = m_to_argument.value(args);
    lf::report_level rl = s_report_level_arg.value(args);
    std::string subject = m_subject_argument.value(args);
    std::string message = m_message_argument.value(args);
    m_engine.file_request(c.server(), c.api_key(), user, subject, message, rl, c.validate_flag());
}

}
