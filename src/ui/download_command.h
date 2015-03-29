#pragma once

#include <cmd/command.h>

namespace lf {
class engine;
}

namespace ui {

/**
 * @class download_command.
 * @brief Class for 'download' command.
 */
class download_command : public cmd::command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    download_command(lf::engine& e);

public:
    /// @brief Executes command by given arguments.
    virtual void execute(const cmd::arguments& args);

private:
    lf::engine& m_engine;
    cmd::argument_definition<std::string, cmd::NAMED_ARGUMENT, false> m_path_argument;
    cmd::argument_definition<std::string, cmd::NAMED_ARGUMENT, false> m_message_id_argument;
    cmd::argument_definition<std::string, cmd::NAMED_ARGUMENT, false> m_sent_in_last_argument;
    cmd::argument_definition<std::string, cmd::NAMED_ARGUMENT, false> m_sent_after_argument;
    cmd::argument_definition<std::string, cmd::UNNAMED_ARGUMENT, false> m_urls_argument;
};

}
