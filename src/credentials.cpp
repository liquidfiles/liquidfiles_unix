#include "credentials.h"
#include "messenger.h"

#include <cstdlib>
#include <errno.h>
#include <fstream>
#include <sys/stat.h>

namespace lf {

namespace credentials {

namespace {

std::string get_directory_path()
{
    std::string h = std::getenv("HOME");
    h += "/.liquidfiles/";
    return h;
}

}

void load(std::string& url, std::string& key, validate_cert& val)
{
    std::string d = get_directory_path();
    d += "/credentials";
    std::ifstream f(d.c_str());
    if (!f) {
        io::mout << "Warning: could not load credentials." << io::endl;
    }
    f >> url;
    if (!f) {
        io::mout << "Warning: could not load credentials." << io::endl;
    }
    f >> key;
    if (!f) {
        io::mout << "Warning: could not load credentials." << io::endl;
    }
    int x;
    f >> x;
    val = static_cast<validate_cert>(x);
}

void save(const std::string& url, const std::string& key, validate_cert val)
{
    std::string d = get_directory_path();
    struct stat sb;
    int x = stat(d.c_str(), &sb);
    if (x != 0 && errno == ENOTDIR) {
        io::mout << "Warning: could not save credentials." << io::endl;
        return;
    }
    if (x != 0) {
        mkdir(d.c_str(), S_IRWXU);
    }
    d += "/credentials";
    std::ofstream f(d.c_str());
    f << url << std::endl;
    f << key << std::endl;
    f << val << std::endl;
}

}

}
