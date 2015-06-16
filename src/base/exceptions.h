#pragma once

#include "exception.h"

#include <string>

namespace base {

class invalid_file_name : public base::exception
{
public:
    invalid_file_name(const std::string& arg)
        : base::exception(std::string("File name '" + arg + "' is invalid."), 5)
    {
    }
};

}
