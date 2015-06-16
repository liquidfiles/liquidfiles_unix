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
class file_request_command : public cmd::command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    file_request_command(lf::engine& e);

public:
    /// @brief Executes command by given arguments.
    virtual void execute(const cmd::arguments& args);

private:
    lf::engine& m_engine;
    cmd::argument_definition<std::string, cmd::NAMED_ARGUMENT, true> m_to_argument;
    cmd::argument_definition<std::string, cmd::NAMED_ARGUMENT, false> m_message_argument;
    cmd::argument_definition<std::string, cmd::NAMED_ARGUMENT, false> m_message_file_argument;
    cmd::argument_definition<std::string, cmd::NAMED_ARGUMENT, false> m_subject_argument;
};

}
