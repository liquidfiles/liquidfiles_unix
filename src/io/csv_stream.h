#pragma once

#include <iostream>
#include <sstream>
#include <string>

namespace io {

/**
 * @class csv_istream.
 * @brief Functionality to read csv format from stream.
 */
class csv_istream
{
public:
    /// @brief Constructor.
    csv_istream(std::istream* s, char d = ',')
        : m_stream(s)
        , m_string()
        , m_position(0)
        , m_delimiter(d)
    {
    }

    /// @brief Read object from stream.
    template<typename T>
    inline csv_istream& operator >> (T& val)
    {
        std::string str = get_delimited_str();
        std::istringstream is(str);
        is >> val;
        return *this;
    }

private:
    char get_delimiter() const
    {
        return m_delimiter;
    }

    std::string get_delimited_str()
    {
        std::string str = "";
        char ch = '\0';
        do {
            if (m_position >= m_string.size()) {
                if(!m_stream->eof()) {
                    std::getline(*m_stream, m_string);
                    m_position = 0;
                } else {
                    m_string = "";
                    break;
                }
                if (!str.empty()) {
                    return str;
                }
            }
            ch = m_string[m_position];
            ++m_position;
            if(ch == m_delimiter || ch == '\r' || ch == '\n') {
                break;
            }
            str += ch;
        } while(true);
        return str;
    }

private:
    std::istream* m_stream;
    std::string m_string;
    size_t m_position;
    char m_delimiter;
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
        : m_stream(s)
        , m_after_newline(true)
        , m_delimiter(d)
    {
    }

    /// @brief Write object to stream.
    template <typename T>
    inline csv_ostream& operator << (const T& val)
    {
        if (!get_after_newline()) {
            get_stream() << get_delimiter();
        }
        get_stream() << val;
        set_after_newline(false);
        return *this;
    }

private:
    char get_delimiter() const
    {
        return m_delimiter;
    }

    void set_after_newline(bool after_newline_)
    {
        m_after_newline = after_newline_;
    }

    bool get_after_newline() const
    {
        return m_after_newline;
    }

    std::ostream& get_stream()
    {
        return *m_stream;
    }

private:
    std::ostream* m_stream;
    bool m_after_newline;
    char m_delimiter;
};

template <>
inline csv_istream& csv_istream::operator >> (std::string& val)
{
    val = get_delimited_str();
    return *this;
}

template <>
inline csv_ostream& csv_ostream::operator << (const char& val)
{
    get_stream() << val;
    if(val == '\n') {
        set_after_newline(true);
    }
    return *this;
}

}
