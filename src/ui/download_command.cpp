#include "download_command.h"
#include "common_arguments.h"
#include "credentials.h"

#include <cmd/exceptions.h>
#include <lf/declarations.h>
#include <lf/engine.h>

namespace ui {

download_command::download_command(lf::engine& e)
    : cmd::command("download", "Download given files.")
    , m_engine(e)
    , m_path_argument("download_to", "<path>", "Directory path to download files there.", "")
    , m_message_id_argument("message_id", "<id>", "Message id to download attachments of it.")
    , m_sent_in_last_argument("sent_in_the_last", "<HOURS>", "Download files sent in the last specified hours.")
    , m_sent_after_argument("sent_after", "YYYYMMDD", "Download files sent after specified date.")
    , m_urls_argument("<url> ...", "Url(s) of files to download.")
{
    get_arguments().push_back(credentials::get_arguments());
    get_arguments().push_back(s_report_level_arg);
    get_arguments().push_back(m_path_argument);
    get_arguments().push_back(m_message_id_argument);
    get_arguments().push_back(m_sent_in_last_argument);
    get_arguments().push_back(m_sent_after_argument);
    get_arguments().push_back(m_urls_argument);
}

void download_command::execute(const cmd::arguments& args)
{
    credentials c;
    try {
        c = credentials::manage(args);
    } catch (cmd::missing_argument&) {
        if (c.api_key() == "") {
            throw;
        }
    }
    std::string path = m_path_argument.value(args);
    lf::report_level rl = s_report_level_arg.value(args);
    std::string l = m_sent_in_last_argument.value(args);
    std::string f = m_sent_after_argument.value(args);
    std::string id = m_message_id_argument.value(args);
    std::set<std::string> unnamed_args = m_urls_argument.value(args);
    if (!c.server().empty()) {
        if (!id.empty()) {
            m_engine.download(c.server(), c.api_key(), path, id, rl, c.validate_flag());
        }
        if (!l.empty() || !f.empty()) {
            m_engine.download(c.server(), c.api_key(), path, l, f, rl, c.validate_flag());
        }
    }
    m_engine.download(unnamed_args, c.api_key(), path, rl, c.validate_flag());
}

}
