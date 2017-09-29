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
class download_command final : public cmd::command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    download_command(lf::engine& e);

public:
    /// @brief Executes command by given arguments.
    void execute(const cmd::arguments& args) override;

private:
    lf::engine& m_engine;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, false> m_path_argument;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, false> m_message_id_argument;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, false> m_sent_in_last_argument;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, false> m_sent_after_argument;
    cmd::argument_definition<std::string, cmd::argument_name_type::unnamed, false> m_urls_argument;
};

}
