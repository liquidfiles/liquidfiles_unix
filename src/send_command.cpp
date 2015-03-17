#include "send_command.h"
#include "credentials.h"
#include "exceptions.h"

#include <lf/declarations.h>
#include <lf/engine.h>

namespace lf {

send_command::send_command(engine& e)
    : command("send",
            credentials::usage() + 
            "[--report_level=<level>] --to=<username> [--subject=<string>] [--message=<string>]\n"
            "\t<file> ...",
            "Sends the file(s) to specified user.",
            credentials::arg_descriptions() + 
            "\t--report_level\n"
            "\t    Level of reporting.\n"
            "\t    Valid values: silent, normal, verbose.\n"
            "\t    Default value: normal.\n\n"
            "\t--to\n"
            "\t    User name or email, to send file.\n\n"
            "\t--subject\n"
            "\t    Subject of composed email. Default value: \"\".\n\n"
            "\t--message\n"
            "\t    Message text of composed email. Default value: \"\".\n\n"
            "\t<file> ...\n"
            "\t    File path(s) to send to user."
            )
    , m_engine(e)
{
}

void send_command::execute(const arguments& args)
{
    credentials c = credentials::manage(args);
    std::string user = args["--to"];
    if (user == "") {
        throw missing_argument("--to");
    }
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
    std::string subject = args["--subject"];
    std::string message = args["--message"];
    const std::set<std::string>& unnamed_args = args.get_unnamed_arguments();
    m_engine.send(c.server(), user, c.api_key(), subject, message, unnamed_args,
            rl, c.validate_flag());
}

}
