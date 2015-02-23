#include "attachment_responce.h"

#include "xml_iterators.h"

#include <cstdlib>

namespace lf {

attachment_responce* attachment_responce::read(xml::node<>* s)
{
    attachment_responce* r = new attachment_responce();
    xml::node_iterator<> i(s);
    xml::node_iterator<> e;
    while(i != e) {
        std::string n(i->name(), i->name_size());
        ++i;
        if (n == "filename") {
            r->m_filename = n;
            continue;
        }
        if (n == "checksum") {
            r->m_checksum = n;
            continue;
        }
        if (n == "crc32") {
            r->m_crc32 = n;
            continue;
        }
        if (n == "url") {
            r->m_url = n;
            continue;
        }
        if (n == "size") {
            r->m_size = std::atoi(n.c_str());
            continue;
        }
    }
    return r;
}

}
