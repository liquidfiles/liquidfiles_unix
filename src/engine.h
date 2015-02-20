#pragma once

#include <curl/curl.h>

#include <set>
#include <string>

namespace lf {

/**
 * @class engine
 * @brief API for liquidfiles.  * *        engine is main class to do operations with liquidfiles.
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
    enum report_level {
        SILENT,
        NORMAL,
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
     * @throw curl_error, upload_error, send_error.
     */
    std::string send(std::string server, const std::string& user, 
            std::string key, const std::string& subject,
            const std::string& message, const files& fs,
            report_level s, validate_cert v);

    /**
     * @brief Lists all the messages.
     * @param server Server URL.
     * @param key API Key of Liquidfiles.
     * @param s Silence flag.
     * @param v Validate certificate flag for HTTP request.
     * @throw curl_error.
     */
    void messages(std::string server, std::string key, report_level s,
            validate_cert v);
    /// @}

private:
    std::string attach(std::string server, const std::string& file,
            report_level s);

    std::string send_attachments(std::string server, const std::string& user,
            const std::string& subject, const std::string& message,
            const files& fs, report_level s);

    void init_curl(std::string key, report_level s, validate_cert v);

private:
    std::string process_send_responce(const std::string& r, report_level s) const;
    void process_attach_responce(const std::string& r, report_level s) const;

private:
    CURL* m_curl;
};

}
