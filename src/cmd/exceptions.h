#pragma once

#include <base/exception.h>

#include <string>

namespace cmd {

class duplicate_name : public base::exception
{
public:
    duplicate_name(const std::string& c)
        : base::exception(std::string("Command with name '") + c +
                    std::string("' already exists."), 100)
    {
    }
};

class missing_argument : public base::exception
{
public:
    missing_argument(const std::string& a)
        : base::exception(std::string("Argument '") + a +
                    std::string("' is required."), 1)
    {
    }
};

class invalid_argument_value : public base::exception
{
public:
    invalid_argument_value(const std::string& arg, const std::string& v)
        : base::exception(std::string("Invalid value for argument '" + arg + "'.\n"
                    "    Valid values: " + v + ".\n"), 1)
    {
    }
};

class invalid_arguments : public base::exception
{
public:
    invalid_arguments(const std::string& m)
        : base::exception(m, 1)
    {
    }
};

class invalid_command_name : public base::exception
{
public:
    invalid_command_name(const std::string& a)
        : base::exception(std::string("Command with name '") + a + "' does not exist.", 1)
    {
    }
};

class dublicate_argument : public base::exception
{
public:
    dublicate_argument(const std::string& a)
        : base::exception(std::string("Only one of '" + a + "' arguments must be persist."), 1)
    {
    }
};

}
