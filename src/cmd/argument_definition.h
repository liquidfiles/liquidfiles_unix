#pragma once

namespace cmd {

class arguments;

class argument_definition
{
public:
    template <typename T>
    argument_definition(const std::string& name,
            const std::string& value_string,
            const std::string& description);

    template <typename T>
    argument_definition(const std::string& name,
            const std::string& description,
            const std::string& value_string,
            const T& default_value);

    template <typename T>
    T get_value(const arguments& a);

    const std::string& name() const
    {
        return m_name;
    }

    const std::string& value_string() const
    {
        return m_value_string;
    }

    const std::string& description() const
    {
        return m_description;
    }

    std::string full_description() const;

private:
    std::string m_name;
    std::string m_value_string;
    std::string m_description;
    std::string m_default_value;
    bool m_is_required;
};

}
