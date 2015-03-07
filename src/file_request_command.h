#pragma once

#include "command.h"

namespace lf {

class engine;

/**
 * @class file_request_command.
 * @brief Class for 'file_request' command.
 */
class file_request_command : public command
{
public:
    /// @brief Constructor.
    /// @param e Engine.
    file_request_command(engine& e);

public:
    /// @brief Executes command by given arguments.
    virtual void execute(const arguments& args);

private:
    engine& m_engine;
};

}
