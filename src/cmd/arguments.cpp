#include "arguments.h"
#include "utility.h"

namespace cmd {

bool arguments::exists(const std::string& n) const
{
    return find(n) != end();
}

const std::string& arguments::operator[](const std::string& n) const
{
    static std::string empty_string;
    const_iterator i = find(n);
    if (i != end()) {
        return i->second;
    }
    return empty_string;
}

const std::set<std::string>& arguments::get_unnamed_arguments() const
{
    return m_unnamed_arguments;
}

const std::set<std::string>& arguments::get_boolean_arguments() const
{
    return m_boolean_arguments;
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
        } else if (utility::is_boolean_argument(*i)) {
            args.m_boolean_arguments.insert(*i);
        } else {
            args.m_unnamed_arguments.insert(*i);
        }
        ++i;
    }
    return args;
}

}
