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

    virtual ~command() = default;

    command(const command&) = delete;
    command& operator=(const command&) = delete;
    /// @}

    /// @name Command parameters.
    /// @{
public:
    std::string usage() const
    {
        return arguments.usage();
    }

public:
    std::string arg_descriptions() const
    {
        return arguments.full_description();
    }
    /// @}

public:
    virtual void execute(const arguments& args) = 0;

public:
    const std::string name;
    const std::string description;
    argument_definition_container arguments;
};

}
