#pragma once

#include <string>

namespace lf {

class exception
{
public:
    exception(std::string m, int c)
        : m_message(m)
        , m_code(c)
    {
    }

public:
    std::string message() const
    {
        return m_message;
    }

    int code() const
    {
        return m_code;
    }

private:
    std::string m_message;
    int m_code;
};

class duplicate_name : public exception
{
public:
    duplicate_name(std::string c)
        : exception(std::string("Command with name '") + c +
                    std::string("' already exists."), 100)
    {
    }
};

class missing_argument : public exception
{
public:
    missing_argument(std::string a)
        : exception(std::string("Argument '") + a +
                    std::string("' is required."), 1)
    {
    }
};

class invalid_argument_value : public exception
{
public:
    invalid_argument_value(std::string arg, std::string v)
        : exception(std::string("Invalid value for argument '" + arg + "'.\n"
                    "    Valid values: " + v + ".\n"), 1)
    {
    }
};

class curl_error : public exception
{
public:
    curl_error(std::string a)
        : exception(a, 2)
    {
    }
};

class upload_error : public exception
{
public:
    upload_error(std::string a)
        : exception(std::string("Error in liquidfiles during upload. Responce message:\n") + a, 3)
    {
    }
};

class send_error : public exception
{
public:
    send_error(std::string a)
        : exception(std::string("Error in liquidfiles during send. Responce message:\n") + a, 4)
    {
    }
};

class file_error : public exception
{
public:
    file_error(std::string f, std::string e)
        : exception(std::string("Can't open file '") + f + "'. " + e, 5)
    {
    }
};

class invalid_command_name : public exception
{
public:
    invalid_command_name(std::string a)
        : exception(std::string("Command with name '") + a + "' does not exist.", 1)
    {
    }
};

class invalid_message_id : public exception
{
public:
    invalid_message_id(std::string a)
        : exception(std::string("Messsage with id '") + a + "' does not exist.", 1)
    {
    }
};

class invalid_url : public exception
{
public:
    invalid_url(std::string a)
        : exception(std::string("Given URL '") + a + "' is invalid.", 1)
    {
    }
};

class get_api_key_error : public exception
{
public:
    get_api_key_error(std::string s)
        : exception(std::string("Error during 'get_api_key' command - ") + s, 1)
    {
    }
};

}
