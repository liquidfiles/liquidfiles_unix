#pragma once

#include "declarations.h"
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
    /**
     * @brief Generates attachment_responce from xml node.
     * @param s Xml node.
     */
    void read(xml::node<>* s);

public:
    /**
     * @brief Gets the string of responce to print.
     * @param f Format of output string.
     */
    std::string to_string(output_format f) const;

public:
    /// @brief Access to filiename.
    const std::string& filename() const
    {
        return m_filename;
    }

    /// @brief Access to content type.
    const std::string& content_type() const
    {
        return m_content_type;
    }
    
    /// @brief Access to checksum.
    const std::string& checksum() const
    {
        return m_checksum;
    }

    /// @brief Access to crc32.
    const std::string& crc32() const
    {
        return m_crc32;
    }

    /// @brief Access to url string.
    const std::string& url() const
    {
        return m_url;
    }

    /// @brief Access to size.
    int size() const
    {
        return m_size;
    }
    
private:
    std::string m_filename;
    std::string m_content_type;
    std::string m_checksum;
    std::string m_crc32;
    std::string m_url;
    int m_size;
};

}
