#pragma once

#include "command.h"

namespace lf {

/**
 * @class send_command.
 * @brief Class for 'send' command.
 */
class send_command : public command
{
public:
    /// @brief Constructor.
    send_command();

public:
    /// @brief Executes command by given arguments.
    virtual void execute(const arguments& args);
};

}
