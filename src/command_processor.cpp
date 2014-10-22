#include "command_processor.h"
#include "command.h"
#include "exceptions.h"

namespace lf {

command_processor* command_processor::s_instance = 0;

command_processor& command_processor::get()
{
    if (s_instance == 0) {
        s_instance = new command_processor();
    }
    return *s_instance;
}

command_processor::command_processor()
{
}

void command_processor::register_command(command* c)
{
    assert(c != 0);
    if (m_commands.find(c->name()) != m_commands.end()) {
        throw duplicate_name(c->name());
    }
    m_commands[c->name()] = c;
}

command* command_processor::get_command(std::string name)
{
    registry::iterator i = m_commands.find(name);
    if (i != m_commands.end()) {
        return i->second;
    }
    return 0;
}

void command_processor::execute(std::string command)
{
    /// @todo
}

}
