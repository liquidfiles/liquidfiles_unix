#pragma once

#include "declarations.h"

#include <cmd/command.h>

namespace lf {
class engine;
}

namespace ui {

/**
 * @class send_command.
 * @brief Class for 'send' command.
 */
class send_command : public cmd::command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    send_command(lf::engine& e);

public:
    /// @brief Executes command by given arguments.
    virtual void execute(const cmd::arguments& args);

private:
    lf::engine& m_engine;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, true> m_to_argument;
    cmd::argument_definition<file_type, cmd::argument_name_type::named, false> m_file_type_argument;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, false> m_message_argument;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, false> m_message_file_argument;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, false> m_subject_argument;
    cmd::argument_definition<std::string, cmd::argument_name_type::unnamed, true> m_files_argument;
};

}
