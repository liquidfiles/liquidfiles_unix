#include "utility.h"

namespace lf {

namespace utility {

void split(std::vector<std::string>& out,
        const std::string& s, const std::string& d)
{
    int i = 0;
    int e = 0;
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
    int i = s.find_first_of(d);
    return i == s.npos ? std::make_pair(s, std::string()) :
        std::make_pair(s.substr(0, i), s.substr(i + 1));
}

}

}
