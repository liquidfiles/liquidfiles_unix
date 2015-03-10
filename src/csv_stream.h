#pragma once

#include <iostream>
#include <sstream>
#include <string>

namespace lf {

/**
 * @class csv_istream.
 * @brief Functionality to read csv format from stream.
 */
class csv_istream
{
public:
    /// @brief Constructor.
    csv_istream(std::istream* s, char d = ',')
        : istm(s)        
        , str()
        , pos(0)
        , delimiter(d)
    {
    }

    /// @brief Read object from stream.
    template<typename T>
    csv_istream& operator>> (T& val)
    {
        std::string str = get_delimited_str();
        std::istringstream is(str);
        is >> val;
        return *this;
    }

private:
    char get_delimiter() const
    {
        return delimiter;
    }

    std::string get_delimited_str()
    {
        std::string str = "";
        char ch = '\0';
        do {
            if(pos>=this->str.size()) {
                if(!istm->eof()) {
                    std::getline(*istm, this->str);
                    pos = 0;
                } else {
                    this->str = "";
                    break;
                }
                if(!str.empty()) {
                    return str;
                }
            }
            ch = this->str[pos];
            ++(pos);
            if(ch==delimiter||ch=='\r'||ch=='\n') {
                break;
            }
            str += ch;
        } while(true);
        return str;
    }

private:
    std::istream* istm;
    std::string str;
    size_t pos;
    char delimiter;
};

/**
 * @class csv_ostream.
 * @brief Functionality to write csv strings.
 */
class csv_ostream
{
public:
    /// @brief Constructor.
    csv_ostream(std::ostream* s, char d = ',')
        : ostm(s)
        , after_newline(true)
        , delimiter(d)
    {
    }

    /// @brief Write object to stream.
    template<typename T>
    csv_ostream& operator<< (const T& val)
    {
        if(!get_after_newline()) {
            get_stream() << get_delimiter();
        }
        get_stream() << val;
        set_after_newline(false);
        return *this;
    }

private:
    char get_delimiter() const
    {
        return delimiter;
    }

    void set_after_newline(bool after_newline_)
    {
        after_newline = after_newline_;
    }

    bool get_after_newline() const
    {
        return after_newline;
    }

    std::ostream& get_stream()
    {
        return *ostm;
    }

private:
    std::ostream* ostm;
    bool after_newline;
    char delimiter;
};

template<>
csv_istream& csv_istream::operator>> (std::string& val)
{
    val = get_delimited_str();
    return *this;
}

template<>
csv_ostream& csv_ostream::operator<< (const char& val)
{
    get_stream() << val;
    if(val == '\n') {
        set_after_newline(true);
    }
    return *this;
}

}
