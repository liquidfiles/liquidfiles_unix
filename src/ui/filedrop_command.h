#pragma once

#include <cmd/command.h>
#include <lf/declarations.h>

namespace lf {
class engine;
}

namespace ui {

/**
 * @class filedrop_command.
 * @brief Class for 'filedrop' command.
 */
class filedrop_command : public cmd::command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    filedrop_command(lf::engine& e);

public:
    /// @brief Executes command by given arguments.
    virtual void execute(const cmd::arguments& args);

private:
    lf::engine& m_engine;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, true> m_server_arg;
    cmd::argument_definition<lf::validate_cert, cmd::argument_name_type::boolean, false> m_validate_cert_arg;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, true> m_from_argument;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, false> m_message_argument;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, false> m_subject_argument;
    cmd::argument_definition<std::string, cmd::argument_name_type::unnamed, true> m_files_argument;
};

}
