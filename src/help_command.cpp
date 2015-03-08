#include "help_command.h"
#include "command_processor.h"
#include "exceptions.h"
#include "messenger.h"
#include "table_printer.h"

namespace lf {

help_command::help_command(command_processor& p)
    : command("help", "[<command_name>]", "Prints help for tool or for specific command.",
        "    <command_name> - Specifies the name of command for which help\n"
        "                     will be printed. If not specified, help for tool\n"
        "                     will be printed."
            )
    , m_command_processor(p)
{
}

void help_command::print_help() const
{
    lf::messenger::get() << "Usage:\n"
    "    liquidfiles <command> <command_args>\n"
"\n"
"Valid commands are:\n";
    std::stringstream ss;
    table_printer tp(&ss);
    tp.add_column("Name", 30);
    tp.add_column("Description", 70);
    tp.print_header();
    std::vector<std::string> cs = m_command_processor.get_command_names();
    std::vector<std::string>::const_iterator i = cs.begin();
    while (i != cs.end()) {
        tp << (*i) << m_command_processor.get_command(*i)->description();
        tp.print_footer();
        ++i;
    }
    messenger::get() << ss.str();

    lf::messenger::get() << "\n"
"Type 'liquidfiles help <command_name>' to see command specific options and usage.\n"
"\n"
"Abnormal exit codes:\n";
    std::stringstream sss;
    table_printer cp(&sss);
    cp.add_column("Code", 5);
    cp.add_column("Description", 140);
    cp.print_header();
    cp << 1 << "Command line arguments are invalid - Invalid command name, missing required argument, invalid value for specific argument.";
    cp.print_footer();
    cp << 2 << "CURL error - Can't connect to host, connection timeout, certificate check failure, etc.";
    cp.print_footer();
    cp << 3 << "Error during file upload - Invalid API key, Invalid filename, etc.";
    cp.print_footer();
    cp << 4 << "Error during file send to user.";
    cp.print_footer();
    cp << 5 << "Error in file system - Can't open file, etc.";
    cp.print_footer();
    messenger::get() << sss.str();
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
        messenger::get() <<
    "Usage:\n"
        "\tliquidfiles " << c->name() << " " <<
        c->usage() << "\n\n"
        "Description:\n\t" << c->description() << "\n\n"
        "Arguments:\n" <<
        c->arg_descriptions() << "\n" << endl;
    }
}

}
