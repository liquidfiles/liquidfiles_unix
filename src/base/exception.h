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
        : m_message(m)
        , m_code(c)
    {
    }

public:
    const std::string& message() const
    {
        return m_message;
    }

    int code() const
    {
        return m_code;
    }

private:
    std::string m_message;
    int m_code;
};

}
