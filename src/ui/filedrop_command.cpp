#include "filedrop_command.h"
#include "credentials.h"
#include "common_arguments.h"

#include <cmd/exceptions.h>
#include <lf/declarations.h>
#include <lf/engine.h>

namespace ui {

filedrop_command::filedrop_command(lf::engine& e)
    : cmd::command("filedrop", "Sends the file(s) by filedrop.")
    , m_engine(e)
    , m_server_arg("server", "<url>", "The server URL.")
    , m_validate_cert_arg("k", "If specified, do not validate server certificate.")
    , m_from_argument("from", "<username>", "User who sends the files")
    , m_message_argument("message", "<string>", "Message text of composed email.", "")
    , m_subject_argument("subject", "<string>", "Subject of composed email.", "")
    , m_attachment_argument("r", "If specified, it means that unnamed arguments are attachment IDs, otherwise they are file paths.")
    , m_files_argument("<file> ...", "File path(s) or attachments IDs to send to user.")
{
    get_arguments().push_back(m_server_arg);
    get_arguments().push_back(m_validate_cert_arg);
    get_arguments().push_back(s_report_level_arg);
    get_arguments().push_back(m_from_argument);
    get_arguments().push_back(m_subject_argument);
    get_arguments().push_back(m_message_argument);
    get_arguments().push_back(m_attachment_argument);
    get_arguments().push_back(m_files_argument);
}

void filedrop_command::execute(const cmd::arguments& args)
{
    std::string server = m_server_arg.value(args);
    lf::validate_cert k = m_validate_cert_arg.value(args);
    lf::report_level rl = s_report_level_arg.value(args);
    std::string user = m_from_argument.value(args);
    std::string subject = m_subject_argument.value(args);
    std::string message = m_message_argument.value(args);
    std::set<std::string> unnamed_args = m_files_argument.value(args);
    bool r = m_attachment_argument.value(args);
    if (r) {
        m_engine.filedrop_attachments(server, user, subject, message, unnamed_args, rl, k);
    } else {
        m_engine.filedrop(server, user, subject, message, unnamed_args, rl, k);
    }
}

}
