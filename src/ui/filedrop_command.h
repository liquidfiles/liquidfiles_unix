#pragma once

#include <cmd/command.h>

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
    cmd::argument_definition<std::string, cmd::NAMED_ARGUMENT, false> m_from_argument;
    cmd::argument_definition<std::string, cmd::NAMED_ARGUMENT, false> m_message_argument;
    cmd::argument_definition<std::string, cmd::NAMED_ARGUMENT, false> m_subject_argument;
    cmd::argument_definition<bool, cmd::BOOLEAN_ARGUMENT, false> m_attachment_argument;
    cmd::argument_definition<std::string, cmd::UNNAMED_ARGUMENT, true> m_files_argument;
};

}
