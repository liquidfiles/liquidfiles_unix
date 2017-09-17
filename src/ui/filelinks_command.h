#pragma once

#include <cmd/command.h>

namespace lf {
class engine;
}

namespace ui {

/**
 * @class filelinks_command.
 * @brief Class for 'filelinks' command.
 */
class filelinks_command final : public cmd::command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    filelinks_command(lf::engine& e);

public:
    /// @brief Executes command by given arguments.
    void execute(const cmd::arguments& args) override;

private:
    lf::engine& m_engine;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, false> m_limit_argument;
};

}
