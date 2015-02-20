#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

namespace lf {

/// @class arguments
/// @brief Represents the arguments of command.
class arguments: public std::map<std::string, std::string>
{
public:
    /// @brief Checks whether argument with given name exists.
    /// @param n Name of argument.
    bool exists(std::string n) const;

public:
    /// @brief Access to argument value by argument name.
    /// @param n Name of argument.
    /// @note If argument with given name does not exist, empty value returned.
    std::string operator[](std::string n) const;

public:
    /// @brief Access to unnamed arguments.
    const std::set<std::string>& get_unnamed_arguments() const;

public:
    /// @brief Constructs and returns arguments from given string.
    static arguments construct(const std::string& str);

    /// @brief Constructs and returns arguments from given strings.
    static arguments construct(const std::vector<std::string>& str);

private:
    std::set<std::string> m_unnamed_arguments;
};

typedef std::string argument_descriptions;

/**
 * @class command
 * @brief Base class for all commands.
 */
class command
{
    /// @name Construction.
    /// @{
public:
    /**
     * @brief Constructs command with given name and given description.
     * @param n Name of command.
     * @param u Usage string.
     * @param d Description of command.
     * @param a Arguments description string.
     */
    command(std::string n, std::string u, std::string d,
            argument_descriptions a);
    
    virtual ~command();

private:
    command(const command&);
    command& operator=(const command&);
    /// @}

    /// @name Command parameters.
    /// @{
public:
    std::string name() const
    {
        return m_name;
    }

public:
    const std::string& usage() const
    {
        return m_usage;
    }

public:
    const std::string& description() const
    {
        return m_description;
    }

public:
    const argument_descriptions& arg_descriptions() const
    {
        return m_argument_descriptions;
    }
    /// @}

public:
    virtual void execute(const arguments& args) = 0;

private:
    const std::string m_name;
    const std::string m_usage;
    const std::string m_description;
    const argument_descriptions m_argument_descriptions;
};

}
