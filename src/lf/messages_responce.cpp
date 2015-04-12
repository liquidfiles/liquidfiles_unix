#include "messages_responce.h"

#include <io/csv_stream.h>
#include <io/table_printer.h>
#include <xml/xml_iterators.h>

namespace lf {

void messages_responce::read(xml::node<>* s)
{
    xml::node_iterator<> ii(s->first_node());
    xml::node_iterator<> e;
    while(ii != e) {
        if (std::string(ii->name(), ii->name_size()) == "message") {
            xml::node_iterator<> i(&*ii);
            m_messages.push_back(message_item());
            message_item& r = m_messages.back();
            while(i != e) {
                std::string n(i->name(), i->name_size());
                std::string v(i->value(), i->value_size());
                xml::node<>* nn = &*i;
                ++i;
                if (n == "id") {
                    r.m_id = v;
                    continue;
                }
                if (n == "sender") {
                    r.m_sender = v;
                    continue;
                }
                if (n == "recipients") {
                    xml::node_iterator<> ri(nn);
                    while(ri != e) {
                        r.m_recipients.push_back(std::string(ri->value(),
                                    ri->value_size()));
                        ++ri;
                    }
                    continue;
                }
                if (n == "created_at") {
                    r.m_creation_time = v;
                    continue;
                }
                if (n == "expires_at") {
                    r.m_expire_time = v;
                    continue;
                }
                if (n == "authorization") {
                    r.m_authorization = std::atoi(v.c_str());
                    continue;
                }
                if (n == "authorization_description") {
                    r.m_authorization_description = v;
                    continue;
                }
                if (n == "subject") {
                    r.m_subject = v;
                    continue;
                }
            }
        }
        ++ii;
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
