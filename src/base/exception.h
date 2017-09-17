#pragma once

#include <string>

namespace base {

/**
 * @class exception.
 * @brief Base class for all exceptions.
 */
class exception
{
public:
    exception(const std::string& m, int c)
        : message{m}
        , code{c}
    {
    }

public:
    const std::string message;
    const int code;
};

}
