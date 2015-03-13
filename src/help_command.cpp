#include "help_command.h"
#include "command_processor.h"
#include "exceptions.h"
#include "messenger.h"

namespace lf {

help_command::help_command(command_processor& p)
    : command("help", "[<command_name>]", "Prints help for tool or for specific command.",
        "\t<command_name> - Specifies the name of command for which help\n"
        "\t                 will be printed. If not specified, help for tool\n"
        "\t                 will be printed."
            )
    , m_command_processor(p)
{
}

void help_command::print_help() const
{
    io::mout << "Usage:\n"
    "\tliquidfiles <command> <command_args>\n"
"\n"
"Valid commands are:\n";
    std::vector<std::string> cs = m_command_processor.get_command_names();
    std::vector<std::string>::const_iterator i = cs.begin();
    unsigned max_length = 0;
    while (i != cs.end()) {
        if (i->size() > max_length) {
            max_length = i->size();
        }
        ++i;
    }
    max_length += 5;
    i = cs.begin();
    while (i != cs.end()) {
        if ((*i) != name()) {
            io::mout << '\t' << (*i);
            for (unsigned j = 0; j < max_length - i->size(); ++j) {
                io::mout << ' ';
            }
            io::mout << m_command_processor.get_command(*i)->description() << io::endl;
        }
        ++i;
    }

    io::mout << "\n"
"Type 'liquidfiles help <command_name>' to see command specific options and usage.\n"
"\n"
"Abnormal exit codes:\n";
    io::mout << '\t' << 1 << "     Command line arguments are invalid - "
        "Invalid command name, missing required argument,"
        " invalid value for specific argument." << io::endl;;
    io::mout << '\t' << 2 << "     CURL error - Can't connect to host,"
        " connection timeout, certificate check failure, etc." << io::endl;;
    io::mout << '\t' << 3 << "     Error during file upload -"
        " Invalid API key, Invalid filename, etc." << io::endl;
    io::mout << '\t' << 4 << "     Error during file send to user." << io::endl;
    io::mout << '\t' << 5 << "     Error in file system - "
        "Can't open file, etc." << io::endl;
}

void help_command::execute(const arguments& args)
{
    if (args.get_unnamed_arguments().empty()) {
        print_help();
        return;
    }
    std::set<std::string>::const_iterator i =
        args.get_unnamed_arguments().begin();
    for (; i != args.get_unnamed_arguments().end(); ++i) {
        command* c = m_command_processor.get_command(*i);
        if (c == 0) {
            throw invalid_command_name(*i);
        }
        io::mout <<
    "Usage:\n"
        "\tliquidfiles " << c->name() << " " <<
        c->usage() << "\n\n"
        "Description:\n\t" << c->description() << "\n\n"
        "Arguments:\n" <<
        c->arg_descriptions() << "\n" << io::endl;
    }
}

}
