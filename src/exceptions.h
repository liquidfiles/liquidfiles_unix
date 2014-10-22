#pragma once

#include <string>

namespace lf {

class exception
{
public:
    exception(std::string m)
        : m_message(m)
    {
    }

public:
    std::string message()
    {
        return m_message;
    }

private:
    std::string m_message;
};

class duplicate_name : public exception
{
public:
    duplicate_name(std::string c)
        : exception(std::string("Command with name '") + c +
                    std::string("' already exists."))
    {
    }
};

}
