#include "delete_filelink_command.h"
#include "download_command.h"
#include "filelink_command.h"
#include "filelinks_command.h"
#include "file_request_command.h"
#include "get_api_key_command.h"
#include "help_command.h"
#include "messages_command.h"
#include "send_command.h"

#include <cmd/command_processor.h>
#include <io/messenger.h>
#include <lf/engine.h>

#include <string>

int main(int argc, char** argv)
{
    lf::engine e;
    cmd::command_processor p(io::mout);
    p.register_command(new lf::help_command(p));
    p.register_command(new lf::send_command(e));
    p.register_command(new lf::messages_command(e));
    p.register_command(new lf::download_command(e));
    p.register_command(new lf::filelink_command(e));
    p.register_command(new lf::delete_filelink_command(e));
    p.register_command(new lf::filelinks_command(e));
    p.register_command(new lf::file_request_command(e));
    p.register_command(new lf::get_api_key_command(e));

    if (argc == 1) {
        p.execute("help");
        return 0;
    }

    std::string c = argv[1];
    std::vector<std::string> args;
    for (int i = 2; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    return p.execute(c, args);
}
