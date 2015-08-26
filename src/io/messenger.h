#pragma once

#include <iostream>

namespace io {

class endl_type{};
class flush_type{};

extern endl_type endl;
extern flush_type flush;

/**
 * @class messenger.
 * @brief Class to write output messages, logs and errors.
 */
class messenger : public std::ostream
{
    /// @name Singleton interface.
    /// @{
public:
    /// @brief Access to singleton instance of messenger.
    static messenger& get()
    {
        if (s_instance == 0) {
            s_instance = new messenger();
        }
        return *s_instance;
    }

private:
    messenger()
    {
    }

    messenger(const messenger&);
    messenger& operator=(const messenger&);

private:
    static messenger* s_instance;
    /// @}

    /// @name Output.
    /// @{
public:
    /// @brief Puts end of line to stream and flushes it.
    messenger& operator<<(endl_type input)
    {
        std::cout << std::endl;
        return *this;
    }

    messenger& operator<<(flush_type input)
    {
        std::cout << std::flush;
        return *this;
    }

    /**
     * @brief Writes given object to the stream.
     * @param t Object to write.
     */
    template <typename T>
    messenger& operator <<(const T& t)
    {
        std::cout << t;
        return *this;
    }
    /// @}
};

extern messenger& mout;

}
