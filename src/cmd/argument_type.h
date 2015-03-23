#pragma once

namespace cmd {

enum argument_type {
    STRING_TYPE,
    INT_TYPE,
    ENUM_TYPE
};

template <argument_type t>
struct enum_to_type;

template <>
struct enum_to_type<STRING_TYPE>
{
    typedef std::string type;
};

template <>
struct enum_to_type<INT_TYPE>
{
    typedef int type;
};

template <typename T>
T string_to_val(const std::string& v);

template <>
std::string string_to_val<std::string>(const std::string& v)
{
    return v;
}

template <>
int string_to_val<int>(const std::string& v)
{
    return std::atoi(v);
}

}
