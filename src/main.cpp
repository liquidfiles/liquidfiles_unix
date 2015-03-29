#include <cmd/command_processor.h>
#include <io/messenger.h>
#include <lf/engine.h>
#include <ui/credentials.h>
#include <ui/delete_filelink_command.h>
#include <ui/download_command.h>
#include <ui/filelink_command.h>
#include <ui/filelinks_command.h>
#include <ui/file_request_command.h>
#include <ui/get_api_key_command.h>
#include <ui/help_command.h>
#include <ui/messages_command.h>
#include <ui/send_command.h>

#include <string>

int main(int argc, char** argv)
{
    lf::engine e;
    cmd::command_processor p(io::mout);
    ui::credentials::init();
    p.register_command(new ui::help_command(p));
    p.register_command(new ui::send_command(e));
    p.register_command(new ui::messages_command(e));
    p.register_command(new ui::download_command(e));
    p.register_command(new ui::filelink_command(e));
    p.register_command(new ui::delete_filelink_command(e));
    p.register_command(new ui::filelinks_command(e));
    p.register_command(new ui::file_request_command(e));
    p.register_command(new ui::get_api_key_command(e));

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
