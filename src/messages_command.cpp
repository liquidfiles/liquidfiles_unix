#include "messages_command.h"
#include "engine.h"
#include "exceptions.h"

namespace lf {

messages_command::messages_command(engine& e)
    : command("messages",
            "[-k] [--report_level=<level>] --server=<url> --api_key=<key>\n"
            "\t(--message_id=<id> | --sent_in_the_last=<HOURS> | --sent_after=<YYYYMMDD>)",
            "Lists the available messages.",
            "\t-k - If specified, don't validate server certificate.\n"
            "\t--report_level - Level of reporting. Valid values:\n"
            "\t                 silent, normal, verbose.\n"
            "\t                 Default value: normal.\n"
            "\t--output_format - Specifies output string format. Valid values:\n"
            "\t                  table, csv.\n"
            "\t                  Default value: table.\n"
            "\t--server - The server URL.\n"
            "\t--api_key - API key of liquidfiles, to login to system.\n"
            "\t--message_id - Message id to download attachments of it.\n"
            "\t--sent_in_the_last - Show messages sent in the last specified\n"
            "\t                     hours.\n"
            "\t--sent_after - Show messages sent after specified date."
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
    output_format of = TABLE_FORMAT;
    std::string ofs = args["--output_format"];
    if (ofs == "csv") {
        of = CSV_FORMAT;
    } else if (ofs != "" && ofs != "table") {
        throw invalid_argument_value("--output_format",
                "table, csv");
    }
    std::set<std::string> unnamed_args = args.get_unnamed_arguments();
    engine::validate_cert val = engine::VALIDATE;
    if (unnamed_args.find("-k") != unnamed_args.end()) {
        val = engine::NOT_VALIDATE;
        unnamed_args.erase("-k");
    }
    std::string id = args["--message_id"];
    if (id == "") {
        m_engine.messages(server, api_key, l, f, rl, val, of);
    } else {
        m_engine.message(server, api_key, id, rl, val, of);
    }
}

}
