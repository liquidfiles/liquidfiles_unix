#pragma once

#include "attachment_responce.h"
#include "declarations.h"

#include "xml/xml.h"

#include <string>
#include <vector>

namespace lf {

/**
 * @class message_responce
 * @brief Class for handling message responce from server and printing
 *        it for user.
 */
class message_responce
{
public:
    /**
     * @brief Generates message_responce from xml node.
     * @param s Xml node.
     */
    void read(xml::node<>* s);

public:
    /**
     * @brief Gets the string of responce to print.
     * @param f Format of string.
     */
    std::string to_string(output_format f) const;

public:
    /// @brief Access to ID.
    const std::string& id() const
    {
        return m_id;
    }

    /// @brief Access to sender.
    const std::string& sender() const
    {
        return m_sender;
    }

    /// @brief Access to recipients.
    const std::vector<std::string>& recipients() const
    {
        return m_recipients;
    }

    /// @brief Access to ccs.
    const std::vector<std::string>& ccs() const
    {
        return m_ccs;
    }

    /// @brief Access to bccs.
    const std::vector<std::string>& bccs() const
    {
        return m_bccs;
    }

    /// @brief Access to creation time.
    const std::string& creation_time() const
    {
        return m_creation_time;
    }

    /// @brief Access to expire time.
    const std::string& expire_time() const
    {
        return m_expire_time;
    }

    /// @brief Access to authorization description.
    int authorization() const
    {
        return m_authorization;
    }

    /// @brief Access to authorization description.
    const std::string& authorization_description() const
    {
        return m_authorization_description;
    }

    /// @brief Access to subject.
    const std::string& subject() const
    {
        return m_subject;
    }

    /// @brief Access to message.
    const std::string& message() const
    {
        return m_message;
    }

    /// @brief Access to attachments.
    const std::vector<attachment_responce>& attachments() const
    {
        return m_attachments;
    }

private:
    void write_table(std::stringstream&) const;
    void write_csv(std::stringstream&) const;

private:
    std::string m_id;
    std::string m_sender;
    std::vector<std::string> m_recipients;
    std::vector<std::string> m_ccs;
    std::vector<std::string> m_bccs;
    std::string m_creation_time;
    std::string m_expire_time;
    std::string m_authorization_description;
    std::string m_subject;
    std::string m_message;
    std::vector<attachment_responce> m_attachments;
    int m_authorization;
};

}
