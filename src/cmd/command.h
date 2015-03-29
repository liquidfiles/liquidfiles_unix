#pragma once

#include "argument_definition.h"
#include "arguments.h"

#include <string>

namespace cmd {

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
     * @param d Description of command.
     */
    command(const std::string& n, const std::string& d);

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
    std::string usage() const
    {
        return m_arguments.usage();
    }

public:
    const std::string& description() const
    {
        return m_description;
    }

public:
    std::string arg_descriptions() const
    {
        return m_arguments.full_description();
    }

protected:
    argument_definition_container& get_arguments()
    {
        return m_arguments;
    }
    /// @}

public:
    virtual void execute(const arguments& args) = 0;

private:
    const std::string m_name;
    const std::string m_description;
    argument_definition_container m_arguments;
};

}
