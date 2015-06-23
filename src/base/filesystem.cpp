#include "filesystem.h"

#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

namespace 
{
void set_all_files(const std::set<std::string>& directory_name, std::set<std::string>& files)
{
    DIR* dir;
    struct dirent* dir_entry;
    std::set<std::string>::const_iterator pos;
    std::set<std::string> sub_dir;

    for (pos = directory_name.begin(); pos != directory_name.end(); ++pos) {
        if ((dir = opendir((*pos).c_str())) == NULL) {
            throw base::opendir_error(*pos);
        } else {
            struct stat stat_buf;
            while ((dir_entry = readdir(dir)) != NULL) {
                std::string file_path = std::string(*pos) + '/' + std::string(dir_entry -> d_name);
                if (dir_entry -> d_name[0] == '.') {
                    continue;
                }
                if (stat(file_path.c_str(), &stat_buf) == 0) {
                    if (S_ISDIR(stat_buf.st_mode)) {
                        sub_dir.insert(file_path);
                    } else if (S_ISREG(stat_buf.st_mode)) {
                        files.insert(file_path);
                    }
                }
            }
        }
        if (closedir(dir) != 0 ) {
            throw base::closedir_error(*pos);
        }
    }
}

}

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

std::set<std::string> filesystem::get_all_files(const std::set<std::string>& directory_name)
{
    std::set<std::string> result;
    set_all_files(directory_name, result);
    return result;
}

}
