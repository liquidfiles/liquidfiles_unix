#include "download_command.h"
#include "engine.h"
#include "exceptions.h"

namespace lf {

download_command::download_command(engine& e)
    : command("download",
            "[-k] [--report_level=<level>] --api_key=<key> --download_to=<path>\n"
            " (--server=<server> --message_id=<id>) | (<url>...) | \n",
            " (--sent_in_the_last=<HOURS>) | (--sent_after=<YYYYMMDD>)"
            "Download given files.",
            "    -k - If specified, don't validate server certificate.\n"
            "    --report_level - Level of reporting. Valid values:\n"
            "                     silent, normal, verbose.\n"
            "                     Default value: normal.\n"
            "    --api_key - API key of liquidfiles, to login to system.\n"
            "    --server - If specified together with --message_id argument,\n"
            "               downloads the attachments of given message.\n"
            "    --message_id - Message id to download attachments of it.\n"
            "    --download_to - Directory path to download files there.\n"
            "    --sent_in_the_last - Download files sent in the last specified \n"
            "                         hours."
            "    <url>... - Urls of files to download.\n"
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
    std::string server = args["--server"];
    std::string id = args["--message_id"];
    std::set<std::string> unnamed_args = args.get_unnamed_arguments();
    engine::validate_cert val = engine::VALIDATE;
    if (unnamed_args.find("-k") != unnamed_args.end()) {
        val = engine::NOT_VALIDATE;
        unnamed_args.erase("-k");
    }
    if (id.empty() || server.empty()) {
        m_engine.download(unnamed_args, path, api_key, rl, val);
    } else {
        m_engine.download(server, path, api_key, id, rl, val);
    }
}

}
