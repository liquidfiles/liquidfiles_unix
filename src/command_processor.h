#pragma once

#include <map>
#include <string>
#include <vector>

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
    /// @name Construction
    /// @{
public:
    /// @brief Constructor.
    /// @param m Messenger.
    command_processor(messenger& m);

    /// @brief Destructor.
    ~command_processor();

private:
    command_processor(const command_processor&);
    command_processor& operator=(const command_processor&);
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
     * @param cn Command string.
     */
    void execute(const std::string& cn);

    /**
     * @brief Executes the given command by given arguments.
     * @param cn Command name.
     * @param args Arguments strings.
     */
    void execute(const std::string& cn,
            const std::vector<std::string>& args);
    /// @}

private:
    typedef std::map<std::string, command*> registry;
    registry m_commands;
    messenger& m_messenger;
};

}
