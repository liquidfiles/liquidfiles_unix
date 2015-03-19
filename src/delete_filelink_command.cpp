#include "delete_filelink_command.h"
#include "credentials.h"
#include "exceptions.h"

#include <lf/declarations.h>
#include <lf/engine.h>

namespace lf {

delete_filelink_command::delete_filelink_command(engine& e)
    : command("delete_filelink",
            credentials::usage() + 
            "[--report_level=<level>] --filelink_id=<id>",
            "Deletes the given filelink.",
            credentials::arg_descriptions() + 
            "\t--report_level\n"
            "\t    Level of reporting.\n"
            "\t    Valid values: silent, normal, verbose.\n"
            "\t    Default value: normal.\n\n"
            "\t--filelink_id\n"
            "\t    ID of filelink to delete."
            )
    , m_engine(e)
{
}

void delete_filelink_command::execute(const arguments& args)
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
    std::string id = args["--filelink_id"];
    if (id == "") {
        throw missing_argument("--filelink_id");
    }
    m_engine.delete_filelink(c.server(), c.api_key(), id,
            rl, c.validate_flag());
}

}
