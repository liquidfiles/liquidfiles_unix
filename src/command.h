#pragma once

#include "arguments.h"

#include <string>

namespace lf {

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
    command(const std::string& n, const std::string& u, const std::string& d,
            const argument_descriptions& a);
    
    virtual ~command();

private:
    command(const command&);
    command& operator=(const command&);
    /// @}

    /// @name Command parameters.
    /// @{
public:
    const std::string& name() const
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
