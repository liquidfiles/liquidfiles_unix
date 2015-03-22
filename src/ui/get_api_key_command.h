#pragma once

#include <cmd/command.h>

namespace lf {
class engine;
}

namespace ui {

/**
 * @class get_api_key_command.
 * @brief Class for 'get_api_key' command.
 */
class get_api_key_command : public cmd::command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    get_api_key_command(lf::engine& e);

public:
    /// @brief Executes command by given arguments.
    virtual void execute(const cmd::arguments& args);

private:
    lf::engine& m_engine;
};

}
