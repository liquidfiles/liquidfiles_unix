#include "download_command.h"
#include "engine.h"
#include "exceptions.h"

namespace lf {

download_command::download_command(engine& e)
    : command("download",
            "[-k] [--report_level=<level>] --api_key=<key> [--download_to=<path>]\n"
            "\t((--server=<server> (--message_id=<id> | --sent_in_the_last=<HOURS> | --sent_after=<YYYYMMDD>)) \n"
            "\t| <url>...)",
            "Download given files.",
            "\t-k - If specified, don't validate server certificate.\n"
            "\t--report_level - Level of reporting. Valid values:\n"
            "\t                 silent, normal, verbose.\n"
            "\t                 Default value: normal.\n"
            "\t--api_key - API key of liquidfiles, to login to system.\n"
            "\t--download_to - Directory path to download files there.\n"
            "\t                Default value: \"\""
            "\t--server - If specified together with --message_id argument,\n"
            "\t           downloads the attachments of given message.\n"
            "\t--message_id - Message id to download attachments of it.\n"
            "\t--sent_in_the_last - Download files sent in the last specified\n"
            "\t                     hours.\n"
            "\t--sent_after - Download files sent after specified date.\n"
            "\t<url>... - Urls of files to download."
            )
    , m_engine(e)
{
}

void download_command::execute(const arguments& args)
{
    std::string api_key = args["--api_key"];
    if (api_key == "") {
        throw missing_argument("--api_key");
    }
    std::string path = args["--download_to"];
    engine::report_level rl = engine::NORMAL;
    std::string rls = args["--report_level"];
    if (rls == "silent") {
        rl = engine::SILENT;
    } else if (rls == "verbose") {
        rl = engine::VERBOSE;
    } else if (rls != "" && rls != "normal") {
        throw invalid_argument_value("--report_level",
                "silent, normal, verbose");
    }
    std::string l = args["--sent_in_the_last"];
    std::string f = args["--sent_after"];
    std::string server = args["--server"];
    std::string id = args["--message_id"];
    std::set<std::string> unnamed_args = args.get_unnamed_arguments();
    engine::validate_cert val = engine::VALIDATE;
    if (unnamed_args.find("-k") != unnamed_args.end()) {
        val = engine::NOT_VALIDATE;
        unnamed_args.erase("-k");
    }
    if (!server.empty()) {
        if (!id.empty()) {
            m_engine.download(server, path, api_key, id, rl, val);
        }
        if (!l.empty() || !f.empty()) {
            m_engine.download(server, path, api_key, l, f, rl, val);
        }
    }
    m_engine.download(unnamed_args, path, api_key, rl, val);
}

}
