#include "utility.h"

namespace lf {

namespace utility {

bool is_named_argument(const std::string& a)
{
    return a.find_first_of("=") != a.npos;
}

void split(std::vector<std::string>& out,
        const std::string& s, const std::string& d)
{
    size_t i = 0;
    size_t e = 0;
    while ((e = s.find_first_of(d, i)) != s.npos) {
        if (e != i) {
            out.push_back(s.substr(i, e - i));
        }
        i = e + 1;
    }
    if (i != s.size()) {
        out.push_back(s.substr(i));
    }
}

std::pair<std::string, std::string>
split(const std::string& s, const std::string& d)
{
    size_t i = s.find_first_of(d);
    return i == s.npos ? std::make_pair(s, std::string()) :
        std::make_pair(s.substr(0, i), s.substr(i + 1));
}

}

}
