#pragma once

#include <sstream>

namespace base {

/**
 * @brief Converts given argument to string.
 * @param t Object to convert to string.
 */
template <typename T>
std::string to_string(T t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

/**
 * @brief Converts given string to given type.
 * @param s string to convert to given type.
 */
template <typename T>
T from_string(std::string s)
{
    std::stringstream ss(s);
    T t;
    ss >> t;
    return t;
}

}
