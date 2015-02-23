#pragma once

#include "xml.h"

#include <string>

namespace lf {

/**
 * @class attachment_responce.
 * @brief Class for handling attachments of message responce.
 */
class attachment_responce
{
public:
    /// @brief Constructor.
    attachment_responce()
    {
    }

public:
    /**
     * @brief Generates attachment_responce from xml node.
     * @param s Xml node.
     */
    static attachment_responce* read(xml::node<>* s);

public:
    /**
     * @brief Writes attachment info to console.
     */
    void write() const;
    
private:
    std::string m_filename;
    std::string m_content_type;
    std::string m_checksum;
    std::string m_crc32;
    std::string m_url;
    int m_size;
};

}
