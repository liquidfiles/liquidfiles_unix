#pragma once

#include <string>
#include <utility>
#include <vector>

namespace lf {

namespace utility {

/**
 * @brief Checks whether argument represented by given string is named.
 * @param a Argument string.
 */
bool is_named_argument(const std::string& a);

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

/**
 * @brief Parses url and returns the filename at the end of it.
 * @param url URL to parse.
 * @throw invalid_url
 */
std::string get_filename(std::string url);
}

}
