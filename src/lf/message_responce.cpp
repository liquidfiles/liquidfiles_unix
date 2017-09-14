#include "message_responce.h"

#include <io/csv_stream.h>
#include <io/table_printer.h>
#include <io/messenger.h>

#include <cstdlib>
#include <sstream>

namespace lf {

void message_responce::read(const nlohmann::json& j)
{
    m_id = j["message"]["id"].get<std::string>();
    m_sender = j["message"]["sender"].get<std::string>();
    m_creation_time = j["message"]["created_at"].get<std::string>();
    m_expire_time = j["message"]["expires_at"].get<std::string>();
    m_authorization = j["message"]["authorization"].get<int>();
    m_authorization_description = j["message"]["authorization_description"].get<std::string>();
    m_subject = j["message"]["subject"].get<std::string>();
    m_message = j["message"]["message"].get<std::string>();
    m_recipients = j["message"]["recipients"].get<std::vector<std::string>>();
    m_ccs = j["message"]["ccs"].get<std::vector<std::string>>();
    if (j["message"].find("bccs") != j["message"].end()) {
        m_bccs = j["message"]["bccs"].get<std::vector<std::string>>();
    }
    auto as = j["message"]["attachments"].get<std::vector<nlohmann::json>>();
    io::mout << as.size() << io::endl;
    for (const auto& a : as) {
        m_attachments.push_back(attachment_responce());
        m_attachments.back().read(a);
    }
}

std::string message_responce::to_string(output_format f) const
{
    std::stringstream m;
    switch (f) {
    case TABLE_FORMAT:
        write_table(m);
        break;
    case CSV_FORMAT:
        write_csv(m);
    default:
        break;
    }
    return m.str();
}

void message_responce::write_table(std::stringstream& m) const
{
    m << "ID: " << m_id << "\n";
    m << "From: " << m_sender << "\n";
    std::vector<std::string>::const_iterator i;
    if (!m_recipients.empty()) {
        m << "To: ";
        i = m_recipients.begin();
        m << *i++;
        while (i != m_recipients.end()) {
            m << ", " << *i++;
        }
        m << "\n";
    }
    if (!m_ccs.empty()) {
        m << "CC: ";
        i = m_ccs.begin();
        m << *i++;
        while (i != m_ccs.end()) {
            m << ", " << *i++;
        }
        m << "\n";
    }
    if (!m_bccs.empty()) {
        m << "BCC: ";
        i = m_bccs.begin();
        m << *i++;
        while (i != m_bccs.end()) {
            m << ", " << *i++;
        }
        m << "\n";
    }
    m << "Created At: " << m_creation_time << "\n";
    m << "Expires At: " << m_expire_time << "\n";
    m << "Authorization: " << m_authorization_description << "\n";
    m << "Subject: " << m_subject << "\n";
    m << "Message: " << m_message << "\n";
    if (!m_attachments.empty()) {
        m << "Attachments:\n";
        io::table_printer tp(&m);
        tp.add_column("N", 4);
        tp.add_column("Attachment", 140);
        tp.print_header();
        std::vector<attachment_responce>::const_iterator j = m_attachments.begin();
        int x = 1;
        while (j != m_attachments.end()) {
            tp << x++;
            std::stringstream ss((j++)->to_string(TABLE_FORMAT));
            std::string s;
            std::getline(ss, s);
            tp << s;
            while (std::getline(ss, s)) {
                tp << " " << s;
            }
            tp.print_footer();
        }
    }
}

void message_responce::write_csv(std::stringstream& m) const
{
    io::csv_ostream cp(&m);
    cp << m_id << m_sender;
    std::vector<std::string>::const_iterator i;
    i = m_recipients.begin();
    cp << m_recipients.size();
    while (i != m_recipients.end()) {
        cp << *i++;
    }
    cp << m_ccs.size();
    i = m_ccs.begin();
    while (i != m_ccs.end()) {
        cp << *i++;
    }
    i = m_bccs.begin();
    cp << m_bccs.size();
    while (i != m_bccs.end()) {
        cp << *i++;
    }
    cp << m_creation_time << m_expire_time << m_authorization_description
        << m_subject << m_message;
    cp << m_attachments.size();
    std::vector<attachment_responce>::const_iterator j = m_attachments.begin();
    while (j != m_attachments.end()) {
        m << ',' << (j++)->to_string(CSV_FORMAT);
    }
}

}
