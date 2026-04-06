#pragma once

#include "declarations.h"

#include <io/json.hpp>

#include <string>
#include <vector>

namespace lf {

/**
 * @class filelinks_response
 * @brief Class for handling filelinks response from server and printing
 *        it for user.
 */
class filelinks_response final
{
public:
    /**
     * @brief Generates filelinks_response from json object.
     * @param j Json object.
     */
    void read(const nlohmann::json& j);

public:
    /**
     * @brief Gets the string representation of response.
     * @param f Output format.
     */
    std::string to_string(output_format f) const;

private:
    struct link_item {
        std::string id;
        std::string filename;
        std::string url;
        std::string expire_time;
        int size;
    };

public:
    using size_type = std::vector<link_item>::size_type;

    /// @brief Returns the count of filelinks.
    size_type size() const
    {
        return m_links.size();
    }

private:
    void write_csv(std::stringstream&) const;
    void write_table(std::stringstream&) const;

private:
    std::vector<link_item> m_links;
};

}
