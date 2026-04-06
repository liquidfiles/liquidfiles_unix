#pragma once

#include "declarations.h"

#include <io/json.hpp>

#include <string>

namespace lf {

/**
 * @class attachment_response.
 * @brief Class for handling attachments of message response.
 */
class attachment_response final
{
public:
    /**
     * @brief Generates attachment_response from json object.
     * @param j Json object
     */
    void read(const nlohmann::json& j);

public:
    /**
     * @brief Gets the string of response to print.
     * @param f Format of output string.
     */
    std::string to_string(output_format f) const;

public:
    /// @brief Access to filename.
    const std::string& filename() const
    {
        return filename_;
    }

    /// @brief Access to content type.
    const std::string& content_type() const
    {
        return content_type_;
    }

    /// @brief Access to checksum.
    const std::string& checksum() const
    {
        return checksum_;
    }

    /// @brief Access to crc32.
    const std::string& crc32() const
    {
        return crc32_;
    }

    /// @brief Access to url string.
    const std::string& url() const
    {
        return url_;
    }

    /// @brief Access to size.
    int size() const
    {
        return size_;
    }

private:
    std::string filename_;
    std::string content_type_;
    std::string checksum_;
    std::string crc32_;
    std::string url_;
    int size_;
};

}
