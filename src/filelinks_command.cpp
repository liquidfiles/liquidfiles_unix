#include "filelinks_command.h"
#include "credentials.h"
#include "exceptions.h"

#include <lf/declarations.h>
#include <lf/engine.h>

namespace lf {

filelinks_command::filelinks_command(engine& e)
    : command("filelinks",
            credentials::usage() + 
            "[--report_level=<level>] [--expires=<YYYY-MM-DD>] <file>",
            "Lists the available filelinks.",
            credentials::arg_descriptions() + 
            "\t--report_level\n"
            "\t    Level of reporting.\n"
            "\t    Valid values: silent, normal, verbose.\n"
            "\t    Default value: normal.\n\n"
            "\t--output_format\n"
            "\t    Specifies output string format.\n"
            "\t    Valid values: table, csv.\n"
            "\t    Default value: table.\n\n"
            "\t--limit\n"
            "\t    Limit of filelinks list."
            )
    , m_engine(e)
{
}

void filelinks_command::execute(const arguments& args)
{
    credentials c = credentials::manage(args);
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
    std::string limit = args["--limit"];
    m_engine.filelinks(c.server(), c.api_key(), limit, rl, c.validate_flag(), of);
}

}
