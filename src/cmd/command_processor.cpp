#include "command_processor.h"
#include "command.h"
#include "exceptions.h"
#include "utility.h"

#include <base/exception.h>
#include <io/messenger.h>

namespace cmd {

command_processor::command_processor(io::messenger& m)
    : m_messenger(m)
{
}

command_processor::~command_processor()
{
    for (const auto& i : m_commands) {
        delete i.second;
    }
    m_commands.clear();
}

void command_processor::register_command(command* c)
{
    if (m_commands.find(c->name) != m_commands.end()) {
        throw duplicate_name(c->name);
    }
    m_commands[c->name] = c;
}

command* command_processor::get_command(std::string name)
{
    registry::iterator i = m_commands.find(name);
    if (i != m_commands.end()) {
        return i->second;
    }
    return 0;
}

int command_processor::execute(const std::string& str)
{
    std::pair<std::string, std::string> p = utility::split(str, " ");
    try {
        command* c = get_command(p.first);
        if (c == 0) {
            throw invalid_command_name(p.first);
        }
        arguments args = arguments::construct(p.second);
        c->execute(args);
    } catch(base::exception& e) {
        m_messenger << "Error: " << e.message << io::endl;
        return e.code;
    } catch(std::exception& e) {
        m_messenger << "Error: " << e.what() << io::endl;
        return 1;
    }
    return 0;
}

int command_processor::execute(const std::string& cn,
        const std::vector<std::string>& args)
{
    try {
        command* c = get_command(cn);
        if (c == 0) {
            throw invalid_command_name(cn);
        }
        arguments a = arguments::construct(args);
        c->execute(a);
    } catch(base::exception& e) {
        m_messenger << "Error: " << e.message << io::endl;
        return e.code;
    } catch(std::exception& e) {
        m_messenger << "Error: " << e.what() << io::endl;
        return 1;
    }
    return 0;
}

}
