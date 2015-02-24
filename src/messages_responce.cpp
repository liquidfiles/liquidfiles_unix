#include "messages_responce.h"
#include "xml_iterators.h"

namespace lf {

messages_responce::~messages_responce()
{
}

messages_responce* messages_responce::read(xml::node<>* s)
{
    messages_responce* r = new messages_responce();
    xml::node_iterator<> i(s);
    xml::node_iterator<> e;
    while(i != e) {
        std::string n(i->name(), i->name_size());
        ++i;
        if (n == "id") {
            continue;
        }
    }
    return r;
}

void messages_responce::write() const
{
}

}
