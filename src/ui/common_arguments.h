#pragma once

#include <cmd/argument_definition.h>
#include <cmd/exceptions.h>
#include <lf/declarations.h>

namespace cmd {

template <>
inline lf::report_level string_to_val(const std::string& v)
{
    if (v == "silent") {
        return lf::report_level::silent;
    } else if (v == "verbose") {
        return lf::report_level::verbose;
    } else if (v == "normal") {
        return lf::report_level::normal;
    }
    throw cmd::invalid_argument_value("--report_level",
            "silent, normal, verbose");
}

template <>
inline std::string val_to_string(const lf::report_level& v)
{
    switch(v) {
        case lf::report_level::normal :
            return "normal";
        case lf::report_level::silent :
            return "silent";
        case lf::report_level::verbose:
            return "verbose";
        default :
            throw 1;
    }
    return "";
}

template <>
inline std::string possible_values<lf::report_level>()
{
    return "Valid values: silent, normal, verbose.";
}

template <>
inline lf::output_format string_to_val(const std::string& v)
{
    if (v == "csv") {
        return lf::output_format::csv;
    } else if (v == "table") {
        return lf::output_format::table;
    }
    throw cmd::invalid_argument_value("--output_format",
            "table, csv");
}

template <>
inline std::string val_to_string(const lf::output_format& v)
{
    switch(v) {
        case lf::output_format::table :
            return "table";
        case lf::output_format::csv :
            return "csv";
        default :
            throw 1;
    }
    return "";
}

template <>
inline std::string possible_values<lf::output_format>()
{
    return "Valid values: table, csv.";
}

}

namespace ui {

extern cmd::argument_definition<lf::report_level, cmd::argument_name_type::named, false> s_report_level_arg;
extern cmd::argument_definition<lf::output_format, cmd::argument_name_type::named, false> s_output_format_arg;
extern cmd::argument_definition<bool, cmd::argument_name_type::boolean, false> s_attachment_argument;

}
