#pragma once

#include "declarations.h"

#include <string>

namespace lf {

namespace credentials {

/**
 * @brief Reads credentials from saved cache and returns it.
 * @param[out] url Url to store here.
 * @param[out] key Api key to store here.
 * @param[out] val Validate certificate to store here.
 */
void load(std::string& url,
        std::string& key,
        validate_cert& val);

/**
 * @brief Saves given credentials in cache.
 * @param url URL to store.
 * @param key Key to store.
 * @param val Validate certificate flag to store.
 */
void save(const std::string& server,
        const std::string& key,
        validate_cert val);

}

}
