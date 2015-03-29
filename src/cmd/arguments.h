#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

namespace cmd {

/// @class arguments
/// @brief Represents the arguments of command.
class arguments: public std::map<std::string, std::string>
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
    /// @brief Access to unnamed arguments.
    const std::set<std::string>& get_unnamed_arguments() const;

    /// @brief Access to boolean arguments.
    const std::set<std::string>& get_boolean_arguments() const;

public:
    /// @brief Constructs and returns arguments from given string.
    static arguments construct(const std::string& str);

    /// @brief Constructs and returns arguments from given strings.
    static arguments construct(const std::vector<std::string>& str);

private:
    std::set<std::string> m_unnamed_arguments;
    std::set<std::string> m_boolean_arguments;
};

}
