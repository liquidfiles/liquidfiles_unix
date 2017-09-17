#include "file_request_command.h"
#include "common_arguments.h"
#include "credentials.h"

#include <cmd/exceptions.h>
#include <base/filesystem.h>
#include <lf/declarations.h>
#include <lf/engine.h>

namespace ui {

file_request_command::file_request_command(lf::engine& e)
    : cmd::command("file_request", "Sends the file request to specified user.")
    , m_engine(e)
    , m_to_argument("to", "<username>", "User name or email, to send file request.")
    , m_message_argument("message", "<string>", "Message text of composed email.", "")
    , m_message_file_argument("message_file", "<string>", "Message text of composed email.", "")
    , m_subject_argument("subject", "<string>", "Subject of composed email.", "")
{
    arguments.push_back(credentials::get_arguments());
    arguments.push_back(s_report_level_arg);
    arguments.push_back(m_to_argument);
    arguments.push_back(m_subject_argument);
    arguments.push_back(m_message_argument);
    arguments.push_back(m_message_file_argument);
}

void file_request_command::execute(const cmd::arguments& args)
{
    credentials c = credentials::manage(args);
    std::string user = m_to_argument.value(args);
    lf::report_level rl = s_report_level_arg.value(args);
    std::string subject = m_subject_argument.value(args);
    std::string message = m_message_argument.value(args);
    std::string message_file = m_message_file_argument.value(args);

    if (!message.empty() && !message_file.empty()) {
        throw cmd::dublicate_argument(m_message_argument.name() + " and " + m_message_file_argument.name());
    }
    if (message.empty() && !message_file.empty()) {
        message = base::filesystem::read_file(message_file);
    }

    m_engine.file_request(c.server(), c.api_key(), user, subject, message, rl, c.validate_flag());
}

}
