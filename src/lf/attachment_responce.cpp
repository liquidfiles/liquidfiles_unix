#include "attachment_responce.h"

#include <io/csv_stream.h>
#include <xml/xml_iterators.h>

#include <cstdlib>
#include <sstream>

namespace lf {

void attachment_responce::read(xml::node<>* s)
{
    xml::node_iterator<> i(s);
    xml::node_iterator<> e;
    while(i != e) {
        std::string n(i->name(), i->name_size());
        std::string v(i->value(), i->value_size());
        ++i;
        if (n == "filename") {
            m_filename = v;
            continue;
        }
        if (n == "checksum") {
            m_checksum = v;
            continue;
        }
        if (n == "crc32") {
            m_crc32 = v;
            continue;
        }
        if (n == "url") {
            m_url = v;
            continue;
        }
        if (n == "size") {
            m_size = std::atoi(v.c_str());
            continue;
        }
    }
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
