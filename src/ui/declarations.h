#pragma once

#include <cmd/argument_type.h>
#include <cmd/exceptions.h>

namespace ui {

enum class file_type {
    names,
    attachment,
    directory
};

}

namespace cmd {

template <>
inline ui::file_type string_to_val(const std::string& v)
{
    if (v == "file_names") {
        return ui::file_type::names;
    } else if (v == "attachment") {
        return ui::file_type::attachment;
    } else if (v == "directory") {
        return ui::file_type::directory;
    }
    throw cmd::invalid_argument_value("--file_type",
            "file_type, attachment, directory");
}

template <>
inline std::string val_to_string(const ui::file_type& v)
{
    switch(v) {
        case ui::file_type::names :
            return "file_names";
        case ui::file_type::attachment :
            return "attachment";
        case ui::file_type::directory :
            return "directory";
        default :
            throw 1;
    }
    return "";
}

}
