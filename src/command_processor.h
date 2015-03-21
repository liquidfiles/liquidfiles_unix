#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <vector>

namespace io {
class messenger;
}

namespace lf {

class command;

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
    command_processor(io::messenger& m);

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

public:
    /**
     * @brief Access to the names of commands by functor.
     */
    template <typename F>
    void for_each_command_name(F f) const;
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
    int execute(const std::string& cn);

    /**
     * @brief Executes the given command by given arguments.
     * @param cn Command name.
     * @param args Arguments strings.
     */
    int execute(const std::string& cn,
            const std::vector<std::string>& args);
    /// @}

private:
    typedef std::map<std::string, command*> registry;
    registry m_commands;
    io::messenger& m_messenger;
};

namespace {

template <typename F>
struct helper_functor
{
    helper_functor(F f)
        : m_unary_function(f)
    {
    }

    void operator()(const std::pair<std::string, command*>& p)
    {
        m_unary_function(p.first);
    }

private:
    F m_unary_function;
};

}

template <typename F>
void command_processor::for_each_command_name(F f) const
{
    helper_functor<F> ff(f);
    std::for_each(m_commands.begin(), m_commands.end(), ff);
}

}
