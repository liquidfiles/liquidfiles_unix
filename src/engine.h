#pragma once

#include <set>
#include <string>

namespace lf {

/**
 * @class engine
 * @brief API for liquidfiles.
 *
 *        engine is main class to do operations with liquidfiles.
 *        It provides interface to send, receive files and other operations
 *        supported by liquidfiles.
 */
class engine
{
public:
    /// @brief Constructor.
    engine();

    /// @brief Destructor.
    ~engine();

private:
    engine(const engine&);
    engine& operator=(const engine&);

    /// @name API
    /// @{
public:
    enum silence {
        SILENT,
        VERBOSE
    };

    enum validate_cert {
        VALIDATE,
        NOT_VALIDATE
    };

    typedef std::set<std::string> files;

public:
    /**
     * @brief Sends the file to specified user, by specified server.
     * @param server Server URL.
     * @param user User name or email.
     * @param key API Key of Liquidfiles.
     * @param subject Subject of composed email.
     * @param message Message body of email.
     * @param fs Files list to send.
     * @param s Silence flag.
     * @param v Validate certificate flag for HTTP request.
     */
    void send(const std::string& server, const std::string& user, 
            const std::string& key, const std::string& subject,
            const std::string& message, const files& fs,
            silence s, validate_cert v);
    /// @}
};

}
