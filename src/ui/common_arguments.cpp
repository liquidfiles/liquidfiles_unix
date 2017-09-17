#include "common_arguments.h"

namespace ui {

cmd::argument_definition<lf::report_level, cmd::argument_name_type::named, false> s_report_level_arg
    ("report_level", "<level>", "Level of reporting.", lf::report_level::normal);

cmd::argument_definition<lf::output_format, cmd::argument_name_type::named, false> s_output_format_arg
    ("output_format", "<format>", "Specifies output string format.", lf::output_format::table);

cmd::argument_definition<bool, cmd::argument_name_type::boolean, false>  s_attachment_argument
    ("r", "If specified, it means that unnamed arguments are attachment IDs,"
     " otherwise they are file paths.");
}
