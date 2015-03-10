#include "message_responce.h"

#include "xml_iterators.h"
#include "table_printer.h"

#include <cstdlib>
#include <sstream>

namespace lf {

void message_responce::read(xml::node<>* s)
{
    xml::node_iterator<> i(s->first_node());
    xml::node_iterator<> e;
    while(i != e) {
        std::string n(i->name(), i->name_size());
        std::string v(i->value(), i->value_size());
        xml::node<>* nn = &*i;
        ++i;
        if (n == "id") {
            m_id = v;
            continue;
        }
        if (n == "sender") {
            m_sender = v;
            continue;
        }
        if (n == "recipients") {
            xml::node_iterator<> ri(nn);
            while(ri != e) {
                m_recipients.push_back(std::string(ri->value(), 
                            ri->value_size()));
                ++ri;
            }
            continue;
        }
        if (n == "ccs") {
            xml::node_iterator<> ri(nn);
            while(ri != e) {
                m_ccs.push_back(std::string(ri->value(), 
                            ri->value_size()));
                ++ri;
            }
            continue;
        }
        if (n == "bccs") {
            xml::node_iterator<> ri(nn);
            while(ri != e) {
                m_bccs.push_back(std::string(ri->value(), 
                            ri->value_size()));
                ++ri;
            }
            continue;
        }
        if (n == "created_at") {
            m_creation_time = v;
            continue;
        }
        if (n == "expires_at") {
            m_expire_time = v;
            continue;
        }
        if (n == "authorization") {
            m_authorization = std::atoi(v.c_str());;
            continue;
        }
        if (n == "authorization_description") {
            m_authorization_description = v;
            continue;
        }
        if (n == "subject") {
            m_subject = v;
            continue;
        }
        if (n == "message") {
            m_message = v;
            continue;
        }
        if (n == "attachments") {
            xml::node_iterator<> ri(nn);
            while(ri != e) {
                m_attachments.push_back(attachment_responce());
                m_attachments.back().read(&*ri);
                ++ri;
            }
            continue;
        }
    }
}

std::string message_responce::to_string(output_format f) const
{
    std::stringstream m;
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
        table_printer tp(&m);
        tp.add_column("N", 4);
        tp.add_column("Attachment", 140);
        tp.print_header();
        std::vector<attachment_responce>::const_iterator j = m_attachments.begin();
        int x = 1;
        while (j != m_attachments.end()) {
            tp << x++;
            std::stringstream ss((j++)->to_string(f));
            std::string s;
            std::getline(ss, s);
            tp << s;
            while (std::getline(ss, s)) {
                tp << " " << s;
            }
            tp.print_footer();
        }
    }
    return m.str();
}

}
