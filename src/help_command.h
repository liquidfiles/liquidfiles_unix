#pragma once

#include "command.h"

#include <map>

namespace lf {

/**
 * @class help_command.
 * @brief Class for 'help' command.
 */
class help_command : public command
{
public:
    /// @brief Constructor.
    help_command();

public:
    /// @brief Executes command by given arguments.
    virtual void execute(const arguments& args);

private:
    void print_help() const;

private:
    std::map<std::string, std::string> m_content;
};

}
