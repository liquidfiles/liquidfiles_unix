#include "command.h"

namespace lf {

command::command(std::string n, std::string u, std::string d,
        argument_descriptions a)
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
