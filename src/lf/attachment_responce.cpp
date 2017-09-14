#include "attachment_responce.h"

#include <io/csv_stream.h>

#include <cstdlib>
#include <sstream>

namespace lf {

void attachment_responce::read(const nlohmann::json& j)
{
    m_filename = j["filename"].get<std::string>();
    m_checksum = j["checksum"].get<std::string>();
    m_crc32 = j["crc32"].get<std::string>();
    m_url = j["url"].get<std::string>();
    m_size = j["size"].get<int>();
}

std::string attachment_responce::to_string(output_format f) const
{
    std::stringstream m;
    switch (f) {
    case TABLE_FORMAT:
        if (!m_filename.empty()) {
            m << "Filename: " << m_filename << "\n";
        }
        if (!m_content_type.empty()) {
            m << "Content Type: " << m_content_type << "\n";
        }
        if (!m_checksum.empty()) {
            m << "Checksum: " << m_checksum << "\n";
        }
        if (!m_crc32.empty()) {
            m << "CRC32: " << m_crc32 << "\n";
        }
        if (!m_url.empty()) {
            m << "URL: " << m_url << "\n";
        }
        m << "Size: " << m_size << "\n";
        break;
    case CSV_FORMAT:
    {
        io::csv_ostream cp(&m);
        cp << m_filename << m_content_type << m_checksum << m_crc32 << m_url << m_size;
    }
    default:
        break;
    }

    return m.str();
}

}
