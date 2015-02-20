#pragma once

#include "command.h"

#include <map>

namespace lf {

class command_processor;

/**
 * @class help_command.
 * @brief Class for 'help' command.
 */
class help_command : public command
{
public:
    /// @brief Constructor.
    help_command(command_processor& p);

public:
    /// @brief Executes command by given arguments.
    virtual void execute(const arguments& args);

private:
    void print_help() const;

private:
    command_processor& m_command_processor;
};

}
