#include "messages_command.h"
#include "credentials.h"
#include "declarations.h"
#include "engine.h"
#include "exceptions.h"

namespace lf {

messages_command::messages_command(engine& e)
    : command("messages",
            credentials::usage() + "[--output_format=<format>]\n"
            "\t[--report_level=<level>] (--message_id=<id> | --sent_in_the_last=<HOURS> | --sent_after=<YYYYMMDD>)",
            "Lists the available messages.",
            credentials::arg_descriptions() +
            "\t--report_level\n"
            "\t    Level of reporting.\n"
            "\t    Valid values: silent, normal, verbose.\n"
            "\t    Default value: normal.\n\n"
            "\t--output_format\n"
            "\t    Specifies output string format.\n"
            "\t    Valid values: table, csv.\n"
            "\t    Default value: table.\n\n"
            "\t--message_id\n"
            "\t    Message id to show.\n\n"
            "\t--sent_in_the_last\n"
            "\t    Show messages sent in the last specified hours.\n\n"
            "\t--sent_after\n"
            "\t    Show messages sent after specified date."
            )
    , m_engine(e)
{
}

void messages_command::execute(const arguments& args)
{
    credentials c = credentials::manage(args);
    std::string l = args["--sent_in_the_last"];
    std::string f = args["--sent_after"];
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
    output_format of = TABLE_FORMAT;
    std::string ofs = args["--output_format"];
    if (ofs == "csv") {
        of = CSV_FORMAT;
    } else if (ofs != "" && ofs != "table") {
        throw invalid_argument_value("--output_format",
                "table, csv");
    }
    std::string id = args["--message_id"];
    if (id == "") {
        m_engine.messages(c.server(), c.api_key(), l, f, rl, c.validate_flag(), of);
    } else {
        m_engine.message(c.server(), c.api_key(), id, rl, c.validate_flag(), of);
    }
}

}
