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

const std::set<std::string>& arguments::get_unnamed_arguments() const
{
    return m_unnamed_arguments;
}

arguments arguments::construct(const std::string& str)
{
    arguments args;
    std::vector<std::string> v;
    utility::split(v, str, " ");
    return construct(v);
}

arguments arguments::construct(const std::vector<std::string>& str)
{
    arguments args;
    std::vector<std::string>::const_iterator i = str.begin();
    while(i != str.end()) {
        if (utility::is_named_argument(*i)) {
            std::pair<std::string, std::string> p = utility::split(*i, "=");
            args.insert(std::make_pair(p.first, p.second));
        } else {
            args.m_unnamed_arguments.insert(*i);
        }
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
