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
class attach_command final : public cmd::command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    attach_command(lf::engine& e);

public:
    /// @brief Executes command by given arguments.
    void execute(const cmd::arguments& args) override;

private:
    lf::engine& m_engine;
    cmd::argument_definition<std::string, cmd::argument_name_type::unnamed, true> m_files_argument;
};

}
