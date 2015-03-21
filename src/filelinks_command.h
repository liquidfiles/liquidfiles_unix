#pragma once

#include <cmd/command.h>

namespace lf {

class engine;

/**
 * @class filelinks_command.
 * @brief Class for 'filelinks' command.
 */
class filelinks_command : public cmd::command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    filelinks_command(engine& e);

public:
    /// @brief Executes command by given arguments.
    virtual void execute(const cmd::arguments& args);

private:
    engine& m_engine;
};

}
