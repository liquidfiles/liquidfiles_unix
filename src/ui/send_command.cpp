#include "send_command.h"
#include "credentials.h"

#include <cmd/exceptions.h>
#include <lf/declarations.h>
#include <lf/engine.h>

namespace ui {

send_command::send_command(lf::engine& e)
    : cmd::command("send",
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

void send_command::execute(const cmd::arguments& args)
{
    credentials c = credentials::manage(args);
    const std::string& user = args["--to"];
    if (user == "") {
        throw cmd::missing_argument("--to");
    }
    lf::report_level rl = lf::NORMAL;
    const std::string& rls = args["--report_level"];
    if (rls == "silent") {
        rl = lf::SILENT;
    } else if (rls == "verbose") {
        rl = lf::VERBOSE;
    } else if (rls != "" && rls != "normal") {
        throw cmd::invalid_argument_value("--report_level",
                "silent, normal, verbose");
    }
    const std::string& subject = args["--subject"];
    const std::string& message = args["--message"];
    const std::set<std::string>& unnamed_args = args.get_unnamed_arguments();
    m_engine.send(c.server(), c.api_key(), user, subject, message, unnamed_args,
            rl, c.validate_flag());
}

}
