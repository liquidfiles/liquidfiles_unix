#pragma once

#include <cmd/argument_definition.h>
#include <cmd/exceptions.h>
#include <lf/declarations.h>

namespace cmd {

template <>
inline lf::report_level string_to_val(const std::string& v)
{
    if (v == "silent") {
        return lf::SILENT;
    } else if (v == "verbose") {
        return lf::VERBOSE;
    } else if (v == "normal") {
        return lf::NORMAL;
    }
    throw cmd::invalid_argument_value("--report_level",
            "silent, normal, verbose");
}

template <>
inline std::string val_to_string(const lf::report_level& v)
{
    switch(v) {
        case lf::NORMAL :
            return "normal";
        case lf::SILENT :
            return "silent";
        case lf::VERBOSE:
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
        return lf::CSV_FORMAT;
    } else if (v == "table") {
        return lf::TABLE_FORMAT;
    }
    throw cmd::invalid_argument_value("--output_format",
            "table, csv");
}

template <>
inline std::string val_to_string(const lf::output_format& v)
{
    switch(v) {
        case lf::TABLE_FORMAT :
            return "table";
        case lf::CSV_FORMAT :
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

extern cmd::argument_definition<lf::report_level, cmd::NAMED_ARGUMENT, false> s_report_level_arg;
extern cmd::argument_definition<lf::output_format, cmd::NAMED_ARGUMENT, false> s_output_format_arg;
extern cmd::argument_definition<bool, cmd::BOOLEAN_ARGUMENT, false> s_attachment_argument;

}
