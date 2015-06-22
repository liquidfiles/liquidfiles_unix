#pragma once

#include "exceptions.h"

#include <fstream>
#include <string>
#include <set>
#include <iterator>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

namespace base {

/**
 * @namespace filesystem.
 * @brief Interface for manipulating with file system.
 */
namespace filesystem
{
    /// @brief Read and return file content.
    /// @param file File name which must be read.
    std::string read_file(const std::string& file);

    /// @brief Recursively read all files in given directory.
    /// @param directory_name Directory name from which must be retrieved files.
    /// @param files Parameter in which inserted all find files absolute path.
    void set_all_files(const std::set<std::string>& directory_name, std::set<std::string>& files);

    /// @brief Recursively read and return all files in given directory.
    /// @param directory_name Directory name from which must be retrieved files.
    std::set<std::string> get_all_files(const std::set<std::string>& directory_name);

}


}
