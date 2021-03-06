#pragma once

#include <cmd/command.h>

namespace lf {
class engine;
}

namespace ui {

/**
 * @class file_request_command.
 * @brief Class for 'file_request' command.
 */
class file_request_command final : public cmd::command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    file_request_command(lf::engine& e);

public:
    /// @brief Executes command by given arguments.
    void execute(const cmd::arguments& args) override;

private:
    lf::engine& m_engine;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, true> m_to_argument;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, false> m_message_argument;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, false> m_message_file_argument;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, false> m_subject_argument;
};

}
