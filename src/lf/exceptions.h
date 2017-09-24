#pragma once

#include <base/exception.h>

#include <string>

namespace lf {

class curl_error final : public base::exception
{
public:
    curl_error(const std::string& a)
        : base::exception{a, 2}
    {
    }
};

class request_error final : public base::exception
{
public:
    request_error(const std::string& r, const std::string& a)
        : base::exception{std::string("Error in liquidfiles during ") + r + ". Responce message:\n" + a, 3}
    {
    }
};

class file_error final : public base::exception
{
public:
    file_error(const std::string& f, const std::string& e)
        : base::exception{std::string("Can't open file '") + f + "'. " + e, 5}
    {
    }
};

class invalid_message_id final : public base::exception
{
public:
    invalid_message_id(const std::string& a)
        : base::exception{std::string("Messsage with id '") + a + "' does not exist.", 1}
    {
    }
};

class invalid_url final : public base::exception
{
public:
    invalid_url(const std::string& a)
        : base::exception{std::string("Given URL '") + a + "' is invalid.", 1}
    {
    }
};

}
