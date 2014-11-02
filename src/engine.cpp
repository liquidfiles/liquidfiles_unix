#include "engine.h"

namespace lf {

engine::engine()
{
}

engine::~engine()
{
}

void engine::send(const std::string& server, const std::string& user,
        const std::string& key, const std::string& subject,
        const std::string& message, const files& fs,
        silence s, validate_cert v)
{
}

}
