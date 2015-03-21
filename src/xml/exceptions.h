#pragma once

#include <base/exception.h>

namespace xml {

/**
 * @class parse_error.
 * @brief Parse error exception. 
 *
 * This exception is thrown by the parser when an error occurs. 
 */
class parse_error: public base::exception
{

public:

    /// @brief Constructor.
    parse_error(const char* what, const char* where)
        : base::exception(std::string("Error during xml parser: '") + what +
                "' in " + std::string(where, 10), 4)
    {
    }
};

}
