#pragma once

#include <cmd/command.h>

namespace lf {
class engine;
}

namespace ui {

/**
 * @class filelink_command.
 * @brief Class for 'filelink' command.
 */
class filelink_command : public cmd::command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    filelink_command(lf::engine& e);

public:
    /// @brief Executes command by given arguments.
    virtual void execute(const cmd::arguments& args);

private:
    lf::engine& m_engine;
    cmd::argument_definition<std::string, cmd::NAMED_ARGUMENT, false> m_expire_argument;
    cmd::argument_definition<std::string, cmd::UNNAMED_ARGUMENT, true> m_file_argument;
};

}
