#include "command_processor.h"
#include "send_command.h"

#include <string>

int main(int argc, char** argv)
{
    lf::command_processor& p = lf::command_processor::get();
    p.register_command(new lf::send_command());

    std::string c;
    for (int i = 1; i < argc; ++i) {
        c += argv[i];
        c += " ";
    }
    p.execute(c);
    return 0;
}
