#include "filelinks_responce.h"

#include <io/csv_stream.h>
#include <io/table_printer.h>

namespace lf {

void filelinks_responce::read(const nlohmann::json& j)
{
    auto ls = j["links"].get<std::vector<nlohmann::json>>();
    for (const auto& l : ls) {
        m_links.emplace_back();
        link_item& r = m_links.back();
        r.m_id = l["id"].get<std::string>();
        r.m_filename = l["filename"].get<std::string>();
        r.m_url = l["url"].get<std::string>();
        r.m_expire_time = l["expires_at"].get<std::string>();
        r.m_size = l["size"].get<int>();
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
