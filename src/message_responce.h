#pragma once

#include "xml.h"

#include <string>
#include <vector>

namespace lf {

class attachment_responce;

/**
 * @class message_responce
 * @brief Class for handling message responce from server and printing
 *        it for user.
 */
class message_responce
{
public:
    /// @brief Constructor.
    message_responce()
    {
    }

    ~message_responce();

public:
    /**
     * @brief Generates message_responce from xml node.
     * @param s Xml node.
     */
    static message_responce* read(xml::node<>* s);

public:
    /**
     * @brief Writes message to console.
     */
    void write() const;

private:
    std::string m_id;
    std::string m_sender;
    std::vector<std::string> m_recipients;
    std::string m_creation_time;
    std::string m_expire_time;
    std::string m_authorization_description;
    std::string m_subject;
    std::string m_message;
    std::vector<attachment_responce*> m_attachments;
    int m_authorization;
};

}
