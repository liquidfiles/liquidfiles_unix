#include "attach_chunk_command.h"
#include "credentials.h"
#include "common_arguments.h"

#include <cmd/exceptions.h>
#include <lf/declarations.h>
#include <lf/engine.h>

namespace ui {

attach_chunk_command::attach_chunk_command(lf::engine& e)
    : cmd::command("attach_chunk", "Uploads given chunk of file to server.")
    , m_engine(e)
    , m_chunk_argument("chunk", "<int>", "ID of current chunk.")
    , m_chunks_argument("chunks", "<int>", "Whole count of chunks.")
    , m_filename_argument("filename", "<string>", "Name of target file.")
    , m_file_argument("<file>", "File chunk path to upload.")
{
    get_arguments().push_back(credentials::get_arguments());
    get_arguments().push_back(s_report_level_arg);
    get_arguments().push_back(m_chunk_argument);
    get_arguments().push_back(m_chunks_argument);
    get_arguments().push_back(m_filename_argument);
    get_arguments().push_back(m_file_argument);
}

void attach_chunk_command::execute(const cmd::arguments& args)
{
    credentials c = credentials::manage(args);
    lf::report_level rl = s_report_level_arg.value(args);
    int chunk = m_chunk_argument.value(args);
    int chunks = m_chunks_argument.value(args);
    std::string filename = m_filename_argument.value(args);
    std::set<std::string> unnamed_args = m_file_argument.value(args);
    if (unnamed_args.size() != 1) {
        throw cmd::invalid_arguments("Need to specify only one file.");
    }
}

}
