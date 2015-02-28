#include "attachment_responce.h"

#include "xml_iterators.h"

#include <cstdlib>
#include <sstream>

namespace lf {

attachment_responce* attachment_responce::read(xml::node<>* s)
{
    attachment_responce* r = new attachment_responce();
    xml::node_iterator<> i(s);
    xml::node_iterator<> e;
    while(i != e) {
        std::string n(i->name(), i->name_size());
        std::string v(i->value(), i->value_size());
        ++i;
        if (n == "filename") {
            r->m_filename = v;
            continue;
        }
        if (n == "checksum") {
            r->m_checksum = v;
            continue;
        }
        if (n == "crc32") {
            r->m_crc32 = v;
            continue;
        }
        if (n == "url") {
            r->m_url = v;
            continue;
        }
        if (n == "size") {
            r->m_size = std::atoi(v.c_str());
            continue;
        }
    }
    return r;
}

std::string attachment_responce::to_string() const
{
    std::stringstream m;
    m << "Filename: " << m_filename << "\n";
    m << "Content Type: " << m_content_type << "\n";
    m << "Checksum: " << m_checksum << "\n";
    m << "CRC32: " << m_crc32 << "\n";
    m << "URL: " << m_url << "\n";
    m << "Size: " << m_size << "\n";
    return m.str();
}

}
