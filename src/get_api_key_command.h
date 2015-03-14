#pragma once

#include "command.h"

namespace lf {

class engine;

/**
 * @class get_api_key_command.
 * @brief Class for 'get_api_key' command.
 */
class get_api_key_command : public command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    get_api_key_command(engine& e);

public:
    /// @brief Executes command by given arguments.
    virtual void execute(const arguments& args);

private:
    engine& m_engine;
};

}
