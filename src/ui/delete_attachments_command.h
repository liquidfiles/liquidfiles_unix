#pragma once

#include <cmd/command.h>

namespace lf {
class engine;
}

namespace ui {

/**
 * @class delete_attachments_command.
 * @brief Class for 'delete_attachments' command.
 */
class delete_attachments_command final : public cmd::command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    delete_attachments_command(lf::engine& e);

public:
    /// @brief Executes command by given arguments.
    void execute(const cmd::arguments& args) override;

private:
    lf::engine& m_engine;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, false> m_message_id_argument;
    cmd::argument_definition<std::string, cmd::argument_name_type::unnamed, false> m_attachment_ids_argument;
};

}
