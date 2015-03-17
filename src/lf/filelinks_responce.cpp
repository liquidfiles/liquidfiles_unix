#include "filelinks_responce.h"

#include <io/csv_stream.h>
#include <io/table_printer.h>
#include <xml/xml_iterators.h>

namespace lf {

void filelinks_responce::read(xml::node<>* s)
{
    xml::node_iterator<> ii(s->first_node());
    xml::node_iterator<> e;
    while(ii != e) {
        if (std::string(ii->name(), ii->name_size()) == "link") {
            xml::node_iterator<> i(&*ii);
            m_links.push_back(link_item());
            link_item& r = m_links.back();
            while(i != e) {
                std::string n(i->name(), i->name_size());
                std::string v(i->value(), i->value_size());
                ++i;
                if (n == "id") {
                    r.m_id = v;
                    continue;
                }
                if (n == "filename") {
                    r.m_filename = v;
                    continue;
                }
                if (n == "url") {
                    r.m_url = v;
                    continue;
                }
                if (n == "expires_at") {
                    r.m_expire_time = v;
                    continue;
                }
                if (n == "size") {
                    r.m_size = v;
                    continue;
                }
            }
        }
        ++ii;
    }
}

std::string filelinks_responce::to_string(output_format f) const
{
    if (m_links.empty()) {
        return std::string();
    }
    std::stringstream m;
    switch (f) {
    case CSV_FORMAT:
        write_csv(m);
        break;
    case TABLE_FORMAT:
        write_table(m);
    default:
        break;
    }
    return m.str();
}

void filelinks_responce::write_csv(std::stringstream& m) const
{
    io::csv_ostream cp(&m);
    std::vector<link_item>::const_iterator j = m_links.begin();
    while (j != m_links.end()) {
        cp << j->m_id << j->m_filename << j->m_size <<
            j->m_expire_time.substr(0, 10) << j->m_url;
        ++j;
    }
}

void filelinks_responce::write_table(std::stringstream& m) const
{
    io::table_printer tp(&m);
    tp.add_column("ID", 24);
    tp.add_column("Filename", 30);
    tp.add_column("Size", 8);
    tp.add_column("Expire Date", 12);
    tp.add_column("URL", 60);
    tp.print_header();
    std::vector<link_item>::const_iterator j = m_links.begin();
    while (j != m_links.end()) {
        tp << j->m_id << j->m_filename << j->m_size <<
            j->m_expire_time.substr(0, 10) << j->m_url;
        ++j;
        tp.print_footer();
    }
}

}
