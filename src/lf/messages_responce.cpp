#include "messages_responce.h"

#include <io/csv_stream.h>
#include <io/table_printer.h>

namespace lf {

void messages_responce::read(const nlohmann::json& j)
{
    auto ms = j["messages"].get<std::vector<nlohmann::json>>();
    for (const auto& mm : ms) {
        m_messages.emplace_back();
        message_item& r = m_messages.back();
        r.m_id = mm["id"].get<std::string>();
        r.m_sender = mm["sender"].get<std::string>();
        r.m_recipients = mm["recipients"].get<std::vector<std::string>>();
        r.m_creation_time = mm["created_at"].get<std::string>();
        r.m_expire_time = mm["expires_at"].get<std::string>();
        r.m_authorization = mm["authorization"].get<int>();
        r.m_authorization_description = mm["authorization_description"].get<std::string>();
        r.m_subject = mm["subject"].get<std::string>();
    }
}

std::string messages_responce::to_string(output_format f) const
{
    if (m_messages.empty()) {
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

void messages_responce::write_csv(std::stringstream& m) const
{
    io::csv_ostream cp(&m);
    std::vector<message_item>::const_iterator j = m_messages.begin();
    while (j != m_messages.end()) {
        cp << j->m_id << j->m_sender;
        unsigned x = 0;
        cp << j->m_recipients.size();
        while (x < j->m_recipients.size()) {
            cp << j->m_recipients[x++];
        }
        cp << j->m_creation_time << j->m_expire_time << j->m_authorization <<
            j->m_subject;
        ++j;
    }
}

void messages_responce::write_table(std::stringstream& m) const
{
    io::table_printer tp(&m);
    tp.add_column("ID", 24);
    tp.add_column("From", 30);
    tp.add_column("To", 30);
    tp.add_column("Create Date", 12);
    tp.add_column("Expire Date", 12);
    tp.add_column("Auth", 5);
    tp.add_column("Subject", 40);
    tp.print_header();
    std::vector<message_item>::const_iterator j = m_messages.begin();
    while (j != m_messages.end()) {
        unsigned x = 0;
        tp << j->m_id << j->m_sender;
        if (x < j->m_recipients.size()) {
            tp << j->m_recipients[x++];
        }
        tp << j->m_creation_time.substr(0, 10) << j->m_expire_time.substr(0, 10) << j->m_authorization <<
            j->m_subject;
        while (x < j->m_recipients.size()) {
            tp << " " << " " << j->m_recipients[x++] << " " << " " << " " << " ";
        }
        ++j;
        tp.print_footer();
    }
}

}
