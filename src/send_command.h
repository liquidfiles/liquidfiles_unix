#pragma once

#include "command.h"

namespace lf {

class engine;

/**
 * @class send_command.
 * @brief Class for 'send' command.
 */
class send_command : public command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    send_command(engine& e);

public:
    /// @brief Executes command by given arguments.
    virtual void execute(const arguments& args);

private:
    engine& m_engine;
};

}
