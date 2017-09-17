#include "command.h"

namespace cmd {

command::command(const std::string& n, const std::string& d)
    : name{n}
    , description{d}
{
}

}
