#pragma once

#include <cmd/command.h>

namespace lf {
class engine;
}

namespace ui {

/**
 * @class attach_chunk_command.
 * @brief Class for 'attach_chunk' command.
 */
class attach_chunk_command final : public cmd::command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    attach_chunk_command(lf::engine& e);

public:
    /// @brief Executes command by given arguments.
    void execute(const cmd::arguments& args) override;

private:
    lf::engine& m_engine;
    cmd::argument_definition<int, cmd::argument_name_type::named, true> m_chunk_argument;
    cmd::argument_definition<int, cmd::argument_name_type::named, true> m_chunks_argument;
    cmd::argument_definition<std::string, cmd::argument_name_type::named, true> m_filename_argument;
    cmd::argument_definition<std::string, cmd::argument_name_type::unnamed, true> m_file_argument;
};

}
