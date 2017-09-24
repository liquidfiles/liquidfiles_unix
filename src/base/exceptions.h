#pragma once

#include "exception.h"

#include <string>

namespace base {

class invalid_file_name : public base::exception
{
public:
    invalid_file_name(const std::string& arg)
        : base::exception(std::string{"File name '" + arg + "' is invalid."}, 5)
    {
    }
};

class opendir_error : public base::exception
{
public:
    opendir_error(const std::string& arg)
        : base::exception(std::string{"Unable to opendir_error directory '" + arg + "'."}, 5)
    {
    }
};

class closedir_error : public base::exception
{
public:
    closedir_error(const std::string& arg)
        : base::exception(std::string{"Unable to close directory '" + arg + "'."}, 5)
    {
    }
};

}
