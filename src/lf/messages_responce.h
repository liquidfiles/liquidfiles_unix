#pragma once

#include "declarations.h"

#include <io/json.h>

#include <string>
#include <vector>

namespace lf {

/**
 * @class messages_responce
 * @brief Class for handling messages responce from server and printing
 *        it for user.
 */
class messages_responce final
{
public:
    /**
     * @brief Generates messages_responce from json object.
     * @param s Xml node.
     */
    void read(const nlohmann::json& s);

public:
    /**
     * @brief Gets the string representation of responce.
     * @param f Output format.
     */
    std::string to_string(output_format f) const;

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

public:
    using size_type = std::vector<message_item>::size_type;

    /// @brief Returns the count of messages.
    size_type size() const
    {
        return m_messages.size();
    }

    /**
     * @brief Returns the id of i-th message.
     * @param i Index of message.
     */
    std::string id(size_type i)
    {
        return m_messages[i].m_id;
    }

private:
    void write_csv(std::stringstream&) const;
    void write_table(std::stringstream&) const;

private:
    std::vector<message_item> m_messages;
};

}
