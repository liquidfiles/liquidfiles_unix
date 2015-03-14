#include "utility.h"
#include "exceptions.h"

namespace lf {

namespace utility {

bool is_named_argument(const std::string& a)
{
    return a.find_first_of("=") != a.npos;
}

bool is_boolean_argument(const std::string& a)
{
    return a.size() > 1 && a[0] == '-' && a[1] != '-';
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

std::string get_filename(std::string url)
{
    std::string::size_type i = url.find_last_of('/');
    if (i == std::string::npos) {
        throw invalid_url(url);
    }
    return url.substr(i + 1);
}

}

}
