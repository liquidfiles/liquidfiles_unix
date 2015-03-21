#pragma once

#include <cmd/command.h>

namespace lf {

class engine;

/**
 * @class messages_command.
 * @brief Class for 'messages' command.
 */
class messages_command : public cmd::command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    messages_command(engine& e);

public:
    /// @brief Executes command by given arguments.
    virtual void execute(const cmd::arguments& args);

private:
    engine& m_engine;
};

}
