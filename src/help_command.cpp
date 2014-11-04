#include "help_command.h"
#include "messenger.h"

namespace lf {

help_command::help_command()
    : command("help", "Help.")
{
    m_content["help"] = "Usage:\n"
        "    liquidfiles help [<command_name>]\n"
        "\n"
        "Description:\n"
        "    Prints help for tool or for specific command.\n"
        "\n"
        "Arguments:\n"
        "    <command_name> - Specifies the name of command for which help\n"
        "                     will be printed. If not specified, help for tool\n"
        "                     will be printed.\n";
    m_content["send"] = "Usage\n"
        "    send [-k] [--report_level=<level>] --to=<username> --server=<url>\n"
        "         --api_key=<key> [--subject=<string>] [--message=<string>]\n"
        "         <file1> [<file2> ...]\n"
        "\n"
        "Description:\n"
        "    Sends the file(s) to specified user.\n"
        "\n"
        "Arguments:\n"
        "    -k - If specified, don't validate server certificate.\n"
        "    --report_level - Level of reporting. Valid values:\n"
        "                     silent, normal, verbose.\n"
        "                     Default value: normal.\n"
        "    --to - User name or email, to send file.\n"
        "    --server - The server URL.\n"
        "    --api_key - API key of liquidfiles, to login to system.\n"
        "    --subject - Subject of composed email. Default value: "".\n"
        "    --message - Message text of composed email. Default value: ""\n"
        "    <file1>... - File path to send to user.\n";
}

void help_command::print_help() const
{
    lf::messenger::get() << "Usage:\n"
    "    liquidfiles <command> <command_args>\n"
"\n"
"Valid commands are:\n"
"send    Sends file to specified user.\n"
"help    Prints help.\n"
"\n"
"Type 'liquidfiles help <command_name>' to see command specific options and usage.";
    lf::messenger::get().endline();
}

void help_command::execute(const arguments& args)
{
    if (args.get_unnamed_arguments().empty()) {
        print_help();
    }
    std::set<std::string>::const_iterator i =
        args.get_unnamed_arguments().begin();
    for (; i != args.get_unnamed_arguments().end(); ++i) {
        messenger::get() << m_content[*i];
        messenger::get().endline();
    }
}

}
