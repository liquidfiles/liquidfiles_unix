#include "command.h"

namespace cmd {

command::command(const std::string& n, const std::string& u, const std::string& d,
        const argument_descriptions& a)
    : m_name(n)
    , m_usage(u)
    , m_description(d)
    , m_argument_descriptions(a)
{
}

command::~command()
{
}

}
