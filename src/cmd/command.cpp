#include "command.h"

namespace cmd {

command::command(const std::string& n, const std::string& d)
    : m_name(n)
    , m_description(d)
{
}

command::~command()
{
}

}
