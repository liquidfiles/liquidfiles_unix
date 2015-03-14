#include "file_request_command.h"
#include "credentials.h"
#include "declarations.h"
#include "engine.h"
#include "exceptions.h"

namespace lf {

file_request_command::file_request_command(engine& e)
    : command("file_request",
            credentials::usage() +
            "[--report_level=<level>] --to=<username> [--subject=<string>] [--message=<string>]",
            "Sends the file request to specified user.",
            credentials::arg_descriptions() +
            "\t--report_level\n"
            "\t    Level of reporting.\n"
            "\t    Valid values: silent, normal, verbose.\n"
            "\t    Default value: normal.\n\n"
            "\t--to\n"
            "\t    User name or email, to send file request.\n\n"
            "\t--subject\n"
            "\t    Subject of composed email. Default value: \"\".\n\n"
            "\t--message\n"
            "\t    Message text of composed email. Default value: \"\"."
            )
    , m_engine(e)
{
}

void file_request_command::execute(const arguments& args)
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
    m_engine.file_request(c.server(), user, c.api_key(), subject, message, rl, c.validate_flag());
}

}
