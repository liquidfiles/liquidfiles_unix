#include "messages_command.h"
#include "engine.h"
#include "exceptions.h"

namespace lf {

messages_command::messages_command(engine& e)
    : command("messages", "Lists the available messages.")
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
    m_engine.messages(server, api_key, rl, val);
}

}
