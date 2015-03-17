#pragma once

#include <base/exception.h>

#include <string>

namespace lf {

class curl_error : public base::exception
{
public:
    curl_error(std::string a)
        : base::exception(a, 2)
    {
    }
};

class upload_error : public base::exception
{
public:
    upload_error(std::string a)
        : base::exception(std::string("Error in liquidfiles during upload. Responce message:\n") + a, 3)
    {
    }
};

class send_error : public base::exception
{
public:
    send_error(std::string a)
        : base::exception(std::string("Error in liquidfiles during send. Responce message:\n") + a, 4)
    {
    }
};

class file_error : public base::exception
{
public:
    file_error(std::string f, std::string e)
        : base::exception(std::string("Can't open file '") + f + "'. " + e, 5)
    {
    }
};

class invalid_message_id : public base::exception
{
public:
    invalid_message_id(std::string a)
        : base::exception(std::string("Messsage with id '") + a + "' does not exist.", 1)
    {
    }
};

class invalid_url : public base::exception
{
public:
    invalid_url(std::string a)
        : base::exception(std::string("Given URL '") + a + "' is invalid.", 1)
    {
    }
};

class get_api_key_error : public base::exception
{
public:
    get_api_key_error(std::string s)
        : base::exception(std::string("Error during 'get_api_key' command - ") + s, 1)
    {
    }
};

}
