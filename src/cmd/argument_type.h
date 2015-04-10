#pragma once

#include <base/string.h>

namespace cmd {

template <typename T>
inline T string_to_val(const std::string& v)
{
    return v;
}

template <typename T>
inline std::string val_to_string(const T& t)
{
    return t;
}

template <typename T>
inline std::string possible_values()
{
    return "";
}

template <>
inline int string_to_val<int>(const std::string& v)
{
    return base::from_string<int>(v);
}

}
