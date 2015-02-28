#include "command_processor.h"
#include "command.h"
#include "exceptions.h"
#include "messenger.h"
#include "utility.h"
#include "xml.h"

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

std::vector<std::string> command_processor::get_command_names() const
{
    std::vector<std::string> res;
    registry::const_iterator i = m_commands.begin();
    while(i != m_commands.end()) {
        res.push_back(i->first);
        ++i;
    }
    return res;
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
    } catch(lf::exception& e) {
        m_messenger << "Error: " << e.message() << endl;
        return e.code();
    } catch(xml::parse_error& e) {
        m_messenger << "Xml error: " << e.what() << endl;
        return 4;
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
    } catch(lf::exception& e) {
        m_messenger << "Error: " << e.message() << endl;
        return e.code();
    } catch(xml::parse_error& e) {
        m_messenger << "Xml error: " << e.what() << endl;
        return 4;
    }
    return 0;
}

}
