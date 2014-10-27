#pragma once

#include <map>
#include <string>

namespace lf {

class command;
class messenger;

/**
 * @class command_processor
 * @brief Central place to store and handle commands.
 *
 *        command_processor is the class which is used to create new commands
 *        and execute them.
 */
class command_processor
{
    /// @name Singleton interface.
    /// @{
public:
    /// @brief Access to singleton instance of command_processor.
    static command_processor& get();

private:
    command_processor();
    command_processor(const command_processor&);
    command_processor& operator=(const command_processor&);
    ~command_processor();

private:
    static command_processor* s_instance;
    /// @}

    /// @name Command register interface
    /// @{
public:
    /**
     * @brief Registers the given command.
     * @param c Command to register.
     * @throw duplicate_name
     */
    void register_command(command* c);

public:
    /**
     * @brief Access to command by given name.
     * @param name Name of command to get.
     * @note If command with given name does not exist, NULL is returned.
     */
    command* get_command(std::string name);
    /// @}

    /// @name Execution
    /// @{
public:
    /**
     * @brief Executes the given string command.
     *         
     *        This function parses given string, gets command name and
     *        executes it by given arguments.
     * @param command Command string.
     */
    void execute(std::string command);
    /// @}

private:
    typedef std::map<std::string, command*> registry;
    registry m_commands;
    messenger& m_messenger;
};

}
