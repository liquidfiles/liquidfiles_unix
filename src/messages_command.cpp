#include "messages_command.h"
#include "engine.h"
#include "exceptions.h"

namespace lf {

messages_command::messages_command(engine& e)
    : command("messages",
            "[-k] [--report_level=<level>] --server=<url> --api_key=<key>\n"
            " (--message_id=<id> | --sent_in_the_last=<HOURS> | --sent_after=<YYYYMMDD>)",
            "Lists the available messages.",
            "    -k - If specified, don't validate server certificate.\n"
            "    --report_level - Level of reporting. Valid values:\n"
            "                     silent, normal, verbose.\n"
            "                     Default value: normal.\n"
            "    --server - The server URL.\n"
            "    --api_key - API key of liquidfiles, to login to system.\n"
            "    --message_id - Message id to download attachments of it.\n"
            "    --sent_in_the_last - Show messages sent in the last specified\n"
            "                         hours.\n"
            "    --sent_after - Show messages sent after specified date."
            )
    , m_engine(e)
{
}

void messages_command::execute(const arguments& args)
{
    std::string server = args["--server"];
    if (server == "") {
        throw missing_argument("--server");
    }
    std::string api_key = args["--api_key"];
    if (api_key == "") {
        throw missing_argument("--api_key");
    }
    std::string l = args["--sent_in_the_last"];
    std::string f = args["--sent_after"];
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
    std::set<std::string> unnamed_args = args.get_unnamed_arguments();
    engine::validate_cert val = engine::VALIDATE;
    if (unnamed_args.find("-k") != unnamed_args.end()) {
        val = engine::NOT_VALIDATE;
        unnamed_args.erase("-k");
    }
    std::string id = args["--message_id"];
    if (id == "") {
        m_engine.messages(server, api_key, l, f, rl, val);
    } else {
        m_engine.message(server, api_key, id, rl, val);
    }
}

}
