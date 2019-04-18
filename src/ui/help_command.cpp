#include "help_command.h"

#include <cmd/command_processor.h>
#include <cmd/exceptions.h>
#include <io/messenger.h>

namespace ui {

help_command::help_command(cmd::command_processor& p)
    : cmd::command{"help", "Prints help for tool or for specific command."}
    , m_command_processor(p)
    , m_commands_argument{"<command_name>", "Specifies the name of command for which help\n"
        "\t                 will be printed. If not specified, help for tool\n"
        "\t                 will be printed."}
{
    arguments.push_back(m_commands_argument);
}

namespace {

class max_length_calculator
{
public:
    max_length_calculator(unsigned& m)
        : m_max_length(m)
    {
    }

public:
    void operator()(const std::string& n)
    {
        if (n.size() > m_max_length) {
            m_max_length = n.size();
        }
    }

private:
    unsigned& m_max_length;
};

class commands_printer
{
public:
    commands_printer(cmd::command_processor& p, const std::string& n, unsigned m)
        : m_command_processor(p)
        , m_skip_name(n)
        , m_max_length(m)
    {
    }

public:
    void operator()(const std::string& n)
    {
        if (n != m_skip_name) {
            io::mout << '\t' << n;
            for (unsigned j = 0; j < m_max_length - n.size(); ++j) {
                io::mout << ' ';
            }
            io::mout << m_command_processor.get_command(n)->description << io::endl;
        }
    }

private:
    cmd::command_processor& m_command_processor;
    const std::string& m_skip_name;
    unsigned m_max_length;
};

}

void help_command::print_help() const
{
    io::mout << "Usage:\n"
    "\tliquidfiles <command> <command_args>\n"
"\n"
"Valid commands are:\n";
    unsigned max_length = 0;
    max_length_calculator ml(max_length);
    m_command_processor.for_each_command_name(ml);
    max_length += 5;
    commands_printer cp(m_command_processor, name, max_length);
    m_command_processor.for_each_command_name(cp);

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
        "Can't open file, etc." << io::endl << io::endl;
}

void help_command::execute(const cmd::arguments& args)
{
    if (args.unnamed_arguments.empty()) {
        print_help();
        return;
    }
    std::set<std::string>::const_iterator i =
        args.unnamed_arguments.begin();
    for (; i != args.unnamed_arguments.end(); ++i) {
        cmd::command* c = m_command_processor.get_command(*i);
        if (c == 0) {
            throw cmd::invalid_command_name(*i);
        }
        io::mout <<
    "Usage:\n"
        "\tliquidfiles " << c->name << " " <<
        c->usage() << "\n\n"
        "Description:\n\t" << c->description << "\n\n"
        "Arguments:\n" <<
        c->arg_descriptions();
    }
}

}
