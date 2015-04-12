#pragma once

#include "declarations.h"

#include <curl/curl.h>

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
    typedef std::set<std::string> strings;

public:
    /**
     * @brief Sends the file to specified user, by specified server.
     * @param server Server URL.
     * @param key API Key of Liquidfiles.
     * @param user User name or email.
     * @param subject Subject of composed email.
     * @param message Message body of email.
     * @param fs Files list to send.
     * @param s Silence flag.
     * @param v Validate certificate flag for HTTP request.
     * @throw curl_error, request_error.
     */
    std::string send(std::string server,
            const std::string& key,
            const std::string& user,
            const std::string& subject,
            const std::string& message,
            const strings& fs,
            report_level s,
            validate_cert v);

    /**
     * @brief Sends the given attachments to given user.
     * @param server Server URL.
     * @param key API Key of Liquidfiles.
     * @param user User name or email.
     * @param subject Subject of composed email.
     * @param message Message body of email.
     * @param fs Attachment IDs.
     * @param s Silence flag.
     * @param v Validate certificate flag for HTTP request.
     * @throw curl_error, request_error.
     */
    std::string send_attachments(std::string server,
            const std::string& key,
            const std::string& user,
            const std::string& subject,
            const std::string& message,
            const strings& fs,
            report_level s,
            validate_cert v);

    /**
     * @brief Uploads given files to server.
     * @param server Server URL.
     * @param key API Key of Liquidfiles.
     * @param fs Files list to send.
     * @param s Silence flag.
     * @param v Validate certificate flag for HTTP request.
     * @throw curl_error, request_error.
     */
    void attach(std::string server,
            const std::string& key,
            const strings& fs,
            report_level s,
            validate_cert v);

    /**
     * @brief Uploads given chunk of the whole file to server.
     * @param server Server URL.
     * @param key API Key of Liquidfiles.
     * @param file Chunk file.
     * @param filename Name of result file.
     * @param chunk_id Id of current chunk.
     * @param num_chunks Count of chunks.
     * @param s Silence flag.
     * @param v Validate certificate flag for HTTP request.
     * @throw curl_error, request_error.
     */
    void attach(std::string server,
            const std::string& key,
            const std::string& file,
            const std::string& filename,
            int chunk_id,
            int num_chunks,
            report_level s,
            validate_cert v);

    /**
     * @brief Lists all the messages.
     * @param server Server URL.
     * @param key API Key of Liquidfiles.
     * @param l Hours, to get messages from the last specified hours.
     * @param f Date, to get messages from that date.
     * @param s Silence flag.
     * @param v Validate certificate flag for HTTP request.
     * @param f Format of output (table or csv).
     * @throw curl_error.
     */
    void messages(std::string server,
            const std::string& key,
            const std::string& l,
            const std::string& f,
            output_format of,
            report_level s,
            validate_cert v);

    /**
     * @brief List the given message.
     * @param server Server URL.
     * @param key API Key of Liquidfiles.
     * @param id Message id.
     * @param s Silence flag.
     * @param v Validate certificate flag for HTTP request.
     * @param f Format of output (table or csv).
     * @throw curl_error, invalid_message_id.
     */
    void message(std::string server,
            const std::string& key,
            const std::string& id,
            output_format f,
            report_level s,
            validate_cert v);

    /**
     * @brief Downloads the files from the given urls.
     * @param urls URLs of the files.
     * @param path Path to output directory.
     * @param key API Key of Liquidfiles.
     * @param s Silence flag.
     * @param v Validate certificate flag for HTTP request.
     * @throw file_error, curl_error, invalid_url.
     */
    void download(const std::set<std::string>& urls,
            const std::string& key,
            const std::string& path,
            report_level s,
            validate_cert v);

    /**
     * @brief Downloads all attachments of the given message.
     * @param server Server URL.
     * @param path Path to output directory.
     * @param key API Key of Liquidfiles.
     * @param id Message id.
     * @param s Silence flag.
     * @param v Validate certificate flag for HTTP request.
     * @throw curl_error, file_error, invalid_message_id, invalid_url.
     */
    void download(std::string server,
            const std::string& key,
            const std::string& path,
            const std::string& id,
            report_level s,
            validate_cert v);

    /**
     * @brief Downloads all attachments of the given messages.
     * @param server Server URL.
     * @param path Path to output directory.
     * @param key API Key of Liquidfiles.
     * @param l Hours, to get messages from the last specified hours.
     * @param f Date, to get messages from that date.
     * @param s Silence flag.
     * @param v Validate certificate flag for HTTP request.
     * @throw curl_error, file_error, invalid_url.
     */
    void download(std::string server,
            const std::string& key,
            const std::string& path,
            const std::string& l,
            const std::string& f,
            report_level s,
            validate_cert v);

    /**
     * @brief Sends the file request to specified user, by specified server.
     * @param server Server URL.
     * @param user User name or email.
     * @param key API Key of Liquidfiles.
     * @param subject Subject of composed email.
     * @param message Message body of email.
     * @param s Silence flag.
     * @param v Validate certificate flag for HTTP request.
     * @throw curl_error, request_error.
     */
    std::string file_request(std::string server,
            const std::string& key,
            const std::string& user,
            const std::string& subject,
            const std::string& message,
            report_level s,
            validate_cert v);

    /**
     * @brief Gets the API key for the specified user.
     * @param server Server URL.
     * @param user User name or email.
     * @param password Password of the user.
     * @param s Silence flag.
     * @param v Validate certificate flag for HTTP request.
     * @throw curl_error, request_error.
     */
    std::string get_api_key(std::string server,
            const std::string& user,
            const std::string& password,
            report_level s,
            validate_cert v);

    /**
     * @brief Uploads given file and create filelink.
     * @param server Server URL.
     * @param key API Key of Liquidfiles.
     * @param file File path to upload.
     * @param s Silence flag.
     * @param v Validate certificate flag for HTTP request.
     * @throw curl_error, request_error.
     */
    std::string filelink(std::string server,
            const std::string& key,
            const std::string& expire,
            const std::string& file,
            report_level s,
            validate_cert v);

    /**
     * @brief Creates filelink on the given attachment.
     * @param server Server URL.
     * @param key API Key of Liquidfiles.
     * @param id Attachment id to create filelink.
     * @param s Silence flag.
     * @param v Validate certificate flag for HTTP request.
     * @throw curl_error, request_error.
     */
    std::string filelink_attachment(std::string server,
            const std::string& key,
            const std::string& expire,
            const std::string& id,
            report_level s,
            validate_cert v);

    /**
     * @brief Deletes the given filelink.
     * @param server Server URL.
     * @param key API Key of Liquidfiles.
     * @param id ID of filelink.
     * @param s Silence flag.
     * @param v Validate certificate flag for HTTP request.
     * @throw curl_error, request_error.
     */
    void delete_filelink(std::string server,
            const std::string& key,
            const std::string& id,
            report_level s,
            validate_cert v);

    /**
     * @brief Lists the filelinks.
     * @param server Server URL.
     * @param key API Key of Liquidfiles.
     * @param limit Limit of list.
     * @param s Silence flag.
     * @param v Validate certificate flag for HTTP request.
     * @throw curl_error.
     */
    void filelinks(std::string server,
            const std::string& key,
            const std::string& limit,
            output_format of,
            report_level s,
            validate_cert v);

    /**
     * @brief Deletes the given attachments.
     * @param server Server URL.
     * @param key API Key of Liquidfiles.
     * @param ids IDs of the attachments.
     * @param s Silence flag.
     * @param v Validate certificate flag for HTTP request.
     * @throw curl_error, invalid_id.
     */
    void delete_attachments(std::string server,
            const std::string& key,
            const std::set<std::string>& ids,
            report_level s,
            validate_cert v);

    /**
     * @brief Deletes all attachments of the given message.
     * @param server Server URL.
     * @param key API Key of Liquidfiles.
     * @param id Message id.
     * @param s Silence flag.
     * @param v Validate certificate flag for HTTP request.
     * @throw curl_error, invalid_message_id.
     */
    void delete_attachments(std::string server,
            const std::string& key,
            const std::string& id,
            report_level s,
            validate_cert v);
    /// @}

private:
    std::string attach_impl(std::string server, const std::string& file,
            report_level s);
    std::string send_attachments_impl(std::string server, const std::string& user,
            const std::string& subject, const std::string& message,
            const strings& fs, report_level s);
    std::string filelink_impl(std::string server, const std::string& expire,
            const std::string& id, report_level s);
    void init_curl(std::string key, report_level s, validate_cert v);
    std::string message_impl(std::string server, const std::string& key, std::string id,
            report_level s, validate_cert v, std::string log);
    std::string messages_impl(std::string server, const std::string& key, std::string l,
            std::string f, report_level s, validate_cert v);
    void download_impl(const std::string& url, const std::string& path, std::string name, report_level s);

private:
    std::string process_send_responce(const std::string& r, report_level s) const;
    void process_attach_responce(const std::string& r, report_level s) const;
    void process_attach_chunk_responce(const std::string& r, report_level s) const;
    std::string process_file_request_responce(const std::string& r, report_level s) const;
    std::string process_get_api_key_responce(const std::string& r, report_level s) const;
    std::string process_create_filelink_responce(const std::string& r, report_level s) const;

    template <typename T>
    void process_output_responce(const std::string& r, report_level s, output_format f) const;

    std::string perform();

private:
    CURL* m_curl;
};

}
