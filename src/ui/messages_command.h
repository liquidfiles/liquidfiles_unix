#pragma once

#include <cmd/command.h>

namespace lf {
class engine;
}

namespace ui {

/**
 * @class messages_command.
 * @brief Class for 'messages' command.
 */
class messages_command : public cmd::command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    messages_command(lf::engine& e);

public:
    /// @brief Executes command by given arguments.
    virtual void execute(const cmd::arguments& args);

private:
    lf::engine& m_engine;
};

}
