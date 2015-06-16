#include "filesystem.h"

namespace base
{

std::string filesystem::read_file(const std::string& file)
{
    std::ifstream file_to_read;
    std::string buffer;
    std::string result;
    file_to_read.open(file.c_str(), std::ifstream::in);
    if (!file_to_read) {
        throw invalid_file_name(file);
    } else {
        while (std::getline(file_to_read, buffer)) {
            result += buffer + '\n';
        }
        file_to_read.close();
    }
    return result;
}

}
