#pragma once

#include "xml.h"

#include <string>
#include <vector>

namespace lf {

/**
 * @class messages_responce
 * @brief Class for handling messages responce from server and printing
 *        it for user.
 */
class messages_responce
{
public:
    /// @brief Constructor.
    messages_responce()
    {
    }

    ~messages_responce();

public:
    /**
     * @brief Generates messages_responce from xml node.
     * @param s Xml node.
     */
    static messages_responce* read(xml::node<>* s);

public:
    /**
     * @brief Writes messages to console.
     */
    std::string to_string() const;

private:
    struct message_item {
        std::string m_id;
        std::string m_sender;
        std::vector<std::string> m_recipients;
        std::string m_creation_time;
        std::string m_expire_time;
        std::string m_authorization_description;
        std::string m_subject;
        int m_authorization;
    };

    std::vector<message_item> m_messages;
};

}
