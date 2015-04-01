#pragma once

#include <cmd/command.h>

namespace lf {
class engine;
}

namespace ui {

/**
 * @class attach_command.
 * @brief Class for 'attach' command.
 */
class attach_command : public cmd::command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    attach_command(lf::engine& e);

public:
    /// @brief Executes command by given arguments.
    virtual void execute(const cmd::arguments& args);

private:
    lf::engine& m_engine;
    cmd::argument_definition<std::string, cmd::UNNAMED_ARGUMENT, true> m_files_argument;
};

}
