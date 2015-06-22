#pragma once

#include <cmd/argument_type.h>
#include <cmd/exceptions.h>

namespace ui {

enum file_type {
    FILE_NAMES,
    ATTACHMENT,
    DIRECTORY
};

}

namespace cmd {

template <>
inline ui::file_type string_to_val(const std::string& v)
{
    if (v == "file_names") {
        return ui::FILE_NAMES;
    } else if (v == "attachment") {
        return ui::ATTACHMENT;
    } else if (v == "directory") {
        return ui::DIRECTORY;
    }
    throw cmd::invalid_argument_value("--file_type",
            "file_type, attachment, directory");
}

template <>
inline std::string val_to_string(const ui::file_type& v)
{
    switch(v) {
        case ui::FILE_NAMES :
            return "file_names";
        case ui::ATTACHMENT :
            return "attachment";
        case ui::DIRECTORY :
            return "directory";
        default :
            throw 1;
    }
    return "";
}

}
