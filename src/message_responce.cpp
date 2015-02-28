#include "message_responce.h"

#include "attachment_responce.h"
#include "xml_iterators.h"
#include "messenger.h"
#include "table_printer.h"

#include <cstdlib>
#include <sstream>

namespace lf {

message_responce::~message_responce()
{
    std::vector<attachment_responce*>::iterator i = m_attachments.begin();
    while (i != m_attachments.end()) {
        delete *i;
    }
}

message_responce* message_responce::read(xml::node<>* s)
{
    message_responce* r = new message_responce();
    xml::node_iterator<> i(s->first_node());
    xml::node_iterator<> e;
    while(i != e) {
        std::string n(i->name(), i->name_size());
        std::string v(i->value(), i->value_size());
        xml::node<>* nn = &*i;
        ++i;
        if (n == "id") {
            r->m_id = v;
            continue;
        }
        if (n == "sender") {
            r->m_sender = v;
            continue;
        }
        if (n == "recipients") {
            xml::node_iterator<> ri(nn);
            while(ri != e) {
                r->m_recipients.push_back(std::string(ri->value(), 
                            ri->value_size()));
                ++ri;
            }
            continue;
        }
        if (n == "ccs") {
            xml::node_iterator<> ri(nn);
            while(ri != e) {
                r->m_ccs.push_back(std::string(ri->value(), 
                            ri->value_size()));
                ++ri;
            }
            continue;
        }
        if (n == "bccs") {
            xml::node_iterator<> ri(nn);
            while(ri != e) {
                r->m_bccs.push_back(std::string(ri->value(), 
                            ri->value_size()));
                ++ri;
            }
            continue;
        }
        if (n == "created_at") {
            r->m_creation_time = v;
            continue;
        }
        if (n == "expires_at") {
            r->m_expire_time = v;
            continue;
        }
        if (n == "authorization") {
            r->m_authorization = std::atoi(v.c_str());;
            continue;
        }
        if (n == "authorization_description") {
            r->m_authorization_description = v;
            continue;
        }
        if (n == "subject") {
            r->m_subject = v;
            continue;
        }
        if (n == "message") {
            r->m_message = v;
            continue;
        }
        if (n == "attachments") {
            xml::node_iterator<> ri(nn);
            while(ri != e) {
                attachment_responce* a = attachment_responce::read(&*ri);
                r->m_attachments.push_back(a);
                ++ri;
            }
            continue;
        }
    }
    return r;
}

std::string message_responce::to_string() const
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
        tp.add_column("Attachment", 100);
        tp.print_header();
        std::vector<attachment_responce*>::const_iterator j = m_attachments.begin();
        int x = 1;
        while (j != m_attachments.end()) {
            tp << x++ << (*j++)->to_string();
        }
        tp.print_footer();
    }
    return m.str();
}

}
