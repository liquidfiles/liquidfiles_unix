#include "attachment_responce.h"

#include <io/csv_stream.h>

#include <cstdlib>
#include <sstream>

namespace lf {

void attachment_responce::read(const nlohmann::json& j)
{
    filename_ = j["filename"].get<std::string>();
    checksum_ = j["checksum"].get<std::string>();
    crc32_ = j["crc32"].get<std::string>();
    url_ = j["url"].get<std::string>();
    size_ = j["size"].get<int>();
}

std::string attachment_responce::to_string(output_format f) const
{
    std::stringstream m;
    switch (f) {
    case output_format::table:
        if (!filename_.empty()) {
            m << "Filename: " << filename_ << "\n";
        }
        if (!content_type_.empty()) {
            m << "Content Type: " << content_type_ << "\n";
        }
        if (!checksum_.empty()) {
            m << "Checksum: " << checksum_ << "\n";
        }
        if (!crc32_.empty()) {
            m << "CRC32: " << crc32_ << "\n";
        }
        if (!url_.empty()) {
            m << "URL: " << url_ << "\n";
        }
        m << "Size: " << size_ << "\n";
        break;
    case output_format::csv:
    {
        io::csv_ostream cp(&m);
        cp << filename_ << content_type_ << checksum_ << crc32_ << url_ << size_;
    }
    default:
        break;
    }

    return m.str();
}

}
