#pragma once

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
    static std::string usage()
    {
        return "[-k] [--server=<url>] [--api_key=<key>] [-s] "; 
    }

    static std::string arg_descriptions()
    {
        return
            "\t-k\n"
            "\t    If specified, do not validate server certificate."
            " If not specified, tries to retrieve from saved credentials.\n\n"
            "\t--server\n"
            "\t    The server URL."
            " If not specified, tries to retrieve from saved credentials.\n\n"
            "\t--api_key\n"
            "\t    API key of liquidfiles, to login to system."
            " If not specified, tries to retrieve from saved credentials.\n\n"
            "\t-s\n"
            "\t    If specified, saves current credentials in cache."
            " Credentials to save are - '-k', '--server' and '--api_key'.\n\n";
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
        , m_validate_flag(lf::VALIDATE)
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
};

}
