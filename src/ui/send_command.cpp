#include "send_command.h"
#include "credentials.h"
#include "common_arguments.h"

#include <cmd/exceptions.h>
#include <base/filesystem.h>
#include <lf/declarations.h>
#include <lf/engine.h>

namespace ui {

send_command::send_command(lf::engine& e)
    : cmd::command{"send", "Sends the file(s) to specified user."}
    , m_engine{e}
    , m_to_argument{"to", "<username>", "User name or email, to send file."}
    , m_file_type_argument{"file_type", "<type>", "Type of unnamed arguments.", file_type::names}
    , m_message_argument{"message", "<string>", "Message text of composed email.", ""}
    , m_message_file_argument{"message_file", "<string>", "Message text of composed email.", ""}
    , m_subject_argument{"subject", "<string>", "Subject of composed email.", ""}
    , m_files_argument{"<file> ...", "File path(s) or attachments IDs to send to user."}
{
    arguments.push_back(credentials::get_arguments());
    arguments.push_back(s_report_level_arg);
    arguments.push_back(m_to_argument);
    arguments.push_back(m_file_type_argument);
    arguments.push_back(m_message_argument);
    arguments.push_back(m_message_file_argument);
    arguments.push_back(m_subject_argument);
    arguments.push_back(m_files_argument);
}

void send_command::execute(const cmd::arguments& args)
{
    credentials c = credentials::manage(args);
    std::string user = m_to_argument.value(args);
    lf::report_level rl = s_report_level_arg.value(args);
    std::string subject = m_subject_argument.value(args);
    std::string message = m_message_argument.value(args);
    std::string message_file = m_message_file_argument.value(args);
    ui::file_type sending_file_type = m_file_type_argument.value(args);

    if (!message.empty() && !message_file.empty()) {
        throw cmd::dublicate_argument(m_message_argument.name() + " and " + m_message_file_argument.name());
    }
    if (message.empty() && !message_file.empty()) {
        message = base::filesystem::read_file(message_file);
    }

    std::set<std::string> unnamed_args = m_files_argument.value(args);

    if (sending_file_type == file_type::attachment) {
        m_engine.send_attachments(c.server(), c.api_key(), user, subject, message, unnamed_args,
                rl, c.validate_flag());
    } else if (sending_file_type == file_type::names) {
        m_engine.send(c.server(), c.api_key(), user, subject, message, unnamed_args,
                rl, c.validate_flag());
    } else if (sending_file_type == file_type::directory) {
        unnamed_args = base::filesystem::get_all_files(unnamed_args);
        m_engine.send(c.server(), c.api_key(), user, subject, message, unnamed_args,
                rl, c.validate_flag());
    }
}

}
