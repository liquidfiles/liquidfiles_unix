#pragma once

#include <cmd/command.h>

namespace lf {
class engine;
}

namespace ui {

/**
 * @class delete_filelink_command.
 * @brief Class for 'delete_filelink' command.
 */
class delete_filelink_command final : public cmd::command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    delete_filelink_command(lf::engine& e);

public:
    /// @brief Executes command by given arguments.
    void execute(const cmd::arguments& args) final;

private:
    lf::engine& m_engine;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, true> m_filelink_id_argument;
};

}
