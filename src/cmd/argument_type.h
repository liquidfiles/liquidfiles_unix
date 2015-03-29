#pragma once

namespace cmd {

template <typename T>
T string_to_val(const std::string& v)
{
    return v;
}

template <typename T>
std::string val_to_string(const T& t)
{
    return t;
}

template <typename T>
std::string possible_values()
{
    return "";
}

}
