#include "command_processor.h"
#include "engine.h"
#include "help_command.h"
#include "messenger.h"
#include "send_command.h"

#include <string>

int main(int argc, char** argv)
{
    lf::engine e;
    lf::command_processor p(lf::messenger::get());
    p.register_command(new lf::send_command(e));
    p.register_command(new lf::help_command());

    if (argc == 1) {
        p.execute("help");
        return 0;
    }

    std::string c = argv[1];
    std::vector<std::string> args;
    for (int i = 2; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    p.execute(c, args);
    return 0;
}
