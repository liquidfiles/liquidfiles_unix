#pragma once

#include "exceptions.h"

#include <string>
#include <fstream>

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

}


}
