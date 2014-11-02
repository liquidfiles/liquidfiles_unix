#include "command_processor.h"
#include "engine.h"
#include "messenger.h"
#include "send_command.h"

#include <string>

int main(int argc, char** argv)
{
    lf::engine e;
    lf::command_processor p(lf::messenger::get());
    p.register_command(new lf::send_command(e));

    std::string c;
    if (argc > 1) {
        c = argv[1];
    }
    std::vector<std::string> args;
    for (int i = 2; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    p.execute(c, args);
    return 0;
}
