#include "command.h"

namespace lf {

bool arguments::exists(std::string n) const
{
    return find(n) != end();
}

std::string arguments::operator[](std::string n) const
{
    iterator i = find(n);
    if (i != end()) {
        return i->second;
    }
    return std::string();
}

arguments arguments::construct(std::string str)
{
    /// @todo
    return arguments();
}

command::command(std::string n, std::string d)
    : m_name(n)
    , m_description(d)
{
}

}
