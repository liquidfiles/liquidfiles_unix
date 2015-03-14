#include "download_command.h"
#include "credentials.h"
#include "declarations.h"
#include "engine.h"
#include "exceptions.h"

namespace lf {

download_command::download_command(engine& e)
    : command("download",
            "[-k] [--api_key=<key>] [-s] [--report_level=<level>] [--download_to=<path>]\n"
            "\t(([--server=<server>] (--message_id=<id> | --sent_in_the_last=<HOURS> | --sent_after=<YYYYMMDD>)) \n"
            "\t| <url>...)",
            "Download given files.",
            credentials::arg_descriptions() +
            "\t--report_level\n"
            "\t    Level of reporting.\n"
            "\t    Valid values: silent, normal, verbose.\n"
            "\t    Default value: normal.\n\n"
            "\t--download_to\n"
            "\t    Directory path to download files there. Default value: \"\"\n\n"
            "\t--message_id\n"
            "\t    Message id to download attachments of it.\n\n"
            "\t--sent_in_the_last\n"
            "\t    Download files sent in the last specified hours.\n\n"
            "\t--sent_after\n"
            "\t    Download files sent after specified date.\n\n"
            "\t<url>...\n"
            "\t    Url(s) of files to download."
            )
    , m_engine(e)
{
}

void download_command::execute(const arguments& args)
{
    credentials c;
    try {
        c = credentials::manage(args);
    } catch (missing_argument&) {
        if (c.api_key() == "") {
            throw;
        }
    }
    std::string path = args["--download_to"];
    report_level rl = NORMAL;
    std::string rls = args["--report_level"];
    if (rls == "silent") {
        rl = SILENT;
    } else if (rls == "verbose") {
        rl = VERBOSE;
    } else if (rls != "" && rls != "normal") {
        throw invalid_argument_value("--report_level",
                "silent, normal, verbose");
    }
    std::string l = args["--sent_in_the_last"];
    std::string f = args["--sent_after"];
    std::string id = args["--message_id"];
    const std::set<std::string>& unnamed_args = args.get_unnamed_arguments();
    if (!c.server().empty()) {
        if (!id.empty()) {
            m_engine.download(c.server(), path, c.api_key(), id, rl, c.validate_flag());
        }
        if (!l.empty() || !f.empty()) {
            m_engine.download(c.server(), path, c.api_key(), l, f, rl, c.validate_flag());
        }
    }
    m_engine.download(unnamed_args, path, c.api_key(), rl, c.validate_flag());
}

}
