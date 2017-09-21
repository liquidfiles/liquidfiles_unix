#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

namespace cmd {

/// @class arguments
/// @brief Represents the arguments of command.
class arguments final
{
public:
    /// @brief Checks whether argument with given name exists.
    /// @param n Name of argument.
    bool exists(const std::string& n) const;

public:
    /// @brief Access to argument value by argument name.
    /// @param n Name of argument.
    /// @note If argument with given name does not exist, empty value returned.
    const std::string& operator[](const std::string& n) const;

public:
    /// @brief Constructs and returns arguments from given string.
    static arguments construct(const std::string& str);

    /// @brief Constructs and returns arguments from given strings.
    static arguments construct(const std::vector<std::string>& str);

public:
    const std::map<std::string, std::string> named_arguments;
    const std::set<std::string> unnamed_arguments;
    const std::set<std::string> boolean_arguments;

private:
    arguments(std::map<std::string, std::string> n,
              std::set<std::string> u,
              std::set<std::string> b)
        : named_arguments{n}
        , unnamed_arguments{u}
        , boolean_arguments{b}
    {
    }
};

}
