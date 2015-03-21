#pragma once

#include <cmd/command.h>

namespace lf {

class engine;

/**
 * @class filelink_command.
 * @brief Class for 'filelink' command.
 */
class filelink_command : public cmd::command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    filelink_command(engine& e);

public:
    /// @brief Executes command by given arguments.
    virtual void execute(const cmd::arguments& args);

private:
    engine& m_engine;
};

}
