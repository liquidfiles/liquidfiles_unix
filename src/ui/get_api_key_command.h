#pragma once

#include <cmd/command.h>
#include <lf/declarations.h>

namespace lf {
class engine;
}

namespace ui {

/**
 * @class get_api_key_command.
 * @brief Class for 'get_api_key' command.
 */
class get_api_key_command final : public cmd::command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    get_api_key_command(lf::engine& e);

public:
    /// @brief Executes command by given arguments.
    void execute(const cmd::arguments& args) override;

private:
    lf::engine& m_engine;
    cmd::argument_definition<lf::validate_cert, cmd::argument_name_type::boolean, false> m_validate_cert_argument;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, true> m_server_argument;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, true> m_username_argument;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, true> m_password_argument;
    cmd::argument_definition<bool, cmd::argument_name_type::boolean, false> m_save_argument;
};

}
