#include "command.h"
#include "utility.h"

namespace lf {

bool arguments::exists(std::string n) const
{
    return find(n) != end();
}

std::string arguments::operator[](std::string n) const
{
    const_iterator i = find(n);
    if (i != end()) {
        return i->second;
    }
    return std::string();
}

arguments arguments::construct(std::string str)
{
    arguments args;
    std::vector<std::string> v;
    utility::split(v, str, " ");
    std::vector<std::string>::const_iterator i = v.begin();
    while(i != v.end()) {
        std::pair<std::string, std::string> p = utility::split(*i, "=");
        args[p.first] = p.second;
        ++i;
    }
    return args;
}

command::command(std::string n, std::string d)
    : m_name(n)
    , m_description(d)
{
}

}
