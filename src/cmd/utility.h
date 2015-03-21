#pragma once

#include <string>
#include <utility>
#include <vector>

namespace cmd {

namespace utility {

/**
 * @brief Checks whether argument represented by given string is named.
 * @param a Argument string.
 */
bool is_named_argument(const std::string& a);

/**
 * @brief Checks whether argument represented by given string is boolean.
 * @param a Argument string.
 */
bool is_boolean_argument(const std::string& a);

/**
 * @brief Splits string to tokens and stores results in output vector.
 * @param s Original string.
 * @param d Delimiter.
 */
void split(std::vector<std::string>& out,
        const std::string& s,
        const std::string& d = " ");

/**
 * @brief Splits string to tokens by the first appearance of delimiter
 *        and returns result pair of strings.
 * @param s Original string.
 * @param d Delimiter.
 */
std::pair<std::string, std::string> split(const std::string& s,
        const std::string& d = " ");
}

}
