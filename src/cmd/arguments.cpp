#include "arguments.h"
#include "utility.h"

namespace cmd {

bool arguments::exists(const std::string& n) const
{
    return named_arguments.find(n) != named_arguments.end();
}

const std::string& arguments::operator[](const std::string& n) const
{
    static std::string empty_string;
    auto i = named_arguments.find(n);
    if (i != named_arguments.end()) {
        return i->second;
    }
    return empty_string;
}

arguments arguments::construct(const std::string& str)
{
    std::vector<std::string> v;
    utility::split(v, str, " ");
    return construct(v);
}

arguments arguments::construct(const std::vector<std::string>& str)
{
    std::map<std::string, std::string> n;
    std::set<std::string> u;
    std::set<std::string> b;
    std::vector<std::string>::const_iterator i = str.begin();
    while (i != str.end()) {
        if (utility::is_named_argument(*i)) {
            std::pair<std::string, std::string> p = utility::split(*i, "=");
            n.insert(std::make_pair(p.first, p.second));
        } else if (utility::is_boolean_argument(*i)) {
            b.insert(*i);
        } else {
            u.insert(*i);
        }
        ++i;
    }
    return arguments{n, u, b};
}

}
