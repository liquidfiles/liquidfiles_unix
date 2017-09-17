#pragma once

#include <cmd/argument_definition.h>
#include <lf/declarations.h>

#include <string>

namespace cmd {
class arguments;
}

namespace ui {

class credentials
{
public:
    /**
     * @brief Manage credentials from the given arguments.
     *        - If '--save' argument is specified, it saves given
     *          credentials.
     *        - If credentials not specified, tries to get it from
     *          saved cache.
     * @param a Arguments.
     * @throw missing_argument.
     */
    static credentials manage(const cmd::arguments& a);

public:
    /**
     * @brief Reads credentials from saved cache and returns it.
     * @param[out] c Credential to load.
     */
    static void load(credentials& c);

    /**
     * @brief Saves given credentials in cache.
     * @param c Credential to save.
     */
    static void save(const credentials& c);

    static const int m_serial_version = 1;

public:
    static void init();

    static const cmd::argument_definition_container& get_arguments()
    {
        return m_arguments;
    }

    static std::string usage()
    {
        return m_arguments.usage();
    }

    static std::string arg_descriptions()
    {
        return m_arguments.full_description();
    }

public:
    /// @brief Access to server.
    const std::string& server() const
    {
        return m_server;
    }

    /// @brief Access to api key.
    const std::string& api_key() const
    {
        return m_api_key;
    }

    /// @brief Access to validate flag.
    lf::validate_cert validate_flag() const
    {
        return m_validate_flag;
    }

public:
    /// @brief Default constructor.
    credentials()
        : m_server()
        , m_api_key()
        , m_validate_flag(lf::validate_cert::validate)
    {
    }

    /**
     * @brief Constructor.
     * @param s Server.
     * @param k Api key.
     * @param v Validate flag.
     */
    credentials(const std::string& s, const std::string k, lf::validate_cert v)
        : m_server(s)
        , m_api_key(k)
        , m_validate_flag(v)
    {
    }

private:
    std::string m_server;
    std::string m_api_key;
    lf::validate_cert m_validate_flag;
    static cmd::argument_definition_container m_arguments;
    static cmd::argument_definition<std::string, cmd::NAMED_ARGUMENT, false> m_server_arg;
    static cmd::argument_definition<std::string, cmd::NAMED_ARGUMENT, false> m_api_key_arg;
    static cmd::argument_definition<lf::validate_cert, cmd::BOOLEAN_ARGUMENT, false> m_validate_cert_arg;
    static cmd::argument_definition<bool, cmd::BOOLEAN_ARGUMENT, false> m_save_arg;
};

}
