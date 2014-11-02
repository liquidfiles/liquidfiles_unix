#include "command_processor.h"
#include "command.h"
#include "exceptions.h"
#include "messenger.h"
#include "utility.h"

#include <cassert>

namespace lf {

command_processor::command_processor(messenger& m)
    : m_messenger(m)
{
}

command_processor::~command_processor()
{
    registry::iterator i = m_commands.begin();
    while (i != m_commands.end()) {
        delete i->second;
        ++i;
    }
    m_commands.clear();
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

void command_processor::execute(const std::string& str)
{
    std::pair<std::string, std::string> p = utility::split(str, " ");
    command* c = get_command(p.first);
    if (c == 0) {
        m_messenger << "Command '" << p.first
            << "' does not exist.";
        m_messenger.endline();
        return;
    }
    arguments args = arguments::construct(p.second);
    try {
        c->execute(args);
    } catch(lf::exception& e) {
        m_messenger << "Error: " << e.message();
        m_messenger.endline();
    }
}

void command_processor::execute(const std::string& cn,
        const std::vector<std::string>& args)
{
    command* c = get_command(cn);
    if (c == 0) {
        m_messenger << "Command '" << cn 
            << "' does not exist.";
        m_messenger.endline();
        return;
    }
    arguments a = arguments::construct(args);
    try {
        c->execute(a);
    } catch(lf::exception& e) {
        m_messenger << "Error: " << e.message();
        m_messenger.endline();
    }
}

}
