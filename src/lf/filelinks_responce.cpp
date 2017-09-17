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
        r.id = l["id"].get<std::string>();
        r.filename = l["filename"].get<std::string>();
        r.url = l["url"].get<std::string>();
        r.expire_time = l["expires_at"].get<std::string>();
        r.size = l["size"].get<int>();
    }
}

std::string filelinks_responce::to_string(output_format f) const
{
    if (m_links.empty()) {
        return std::string();
    }
    std::stringstream m;
    switch (f) {
    case output_format::csv:
        write_csv(m);
        break;
    case output_format::table:
        write_table(m);
    default:
        break;
    }
    return m.str();
}

void filelinks_responce::write_csv(std::stringstream& m) const
{
    io::csv_ostream cp(&m);
    for (const auto& l : m_links) {
        cp << l.id << l.filename << l.size << l.expire_time.substr(0, 10) << l.url;
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
    for (const auto& l : m_links) {
        tp << l.id << l.filename << l.size << l.expire_time.substr(0, 10) << l.url;
        tp.print_footer();
    }
}

}
