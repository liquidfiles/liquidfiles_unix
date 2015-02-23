#include "message_responce.h"

#include "attachment_responce.h"
#include "xml_iterators.h"

#include <cstdlib>

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
    xml::node_iterator<> i(s);
    xml::node_iterator<> e;
    while(i != e) {
        std::string n(i->name(), i->name_size());
        xml::node<>* nn = &*i;
        ++i;
        if (n == "id") {
            r->m_id = n;
            continue;
        }
        if (n == "sender") {
            r->m_sender = n;
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
        if (n == "created_at") {
            r->m_creation_time = n;
            continue;
        }
        if (n == "expires_at") {
            r->m_expire_time = n;
            continue;
        }
        if (n == "authorization") {
            r->m_authorization = std::atoi(n.c_str());;
            continue;
        }
        if (n == "authorization_description") {
            r->m_authorization_description = n;
            continue;
        }
        if (n == "subject") {
            r->m_subject = n;
            continue;
        }
        if (n == "message") {
            r->m_message = n;
            continue;
        }
    }
    return r;
}

void message_responce::write() const
{
}

}
