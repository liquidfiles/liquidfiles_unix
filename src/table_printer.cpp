#include "table_printer.h"

#include <iomanip>
#include <stdexcept>

namespace lf {

table_printer::table_printer(std::ostream* output,
        const std::string& separator)
    : m_out_stream(output)
    , m_separator(separator)
    , m_i(0)
    , m_j(0)
{
}

int table_printer::get_num_columns() const
{
    return m_column_headers.size();
}

void table_printer::add_column(const std::string & header_name, int column_width)
{
    if (column_width < 4) {
        throw std::invalid_argument("Column size has to be >= 4");
    }

    m_column_headers.push_back(header_name);
    m_column_widths.push_back(column_width);
}

void table_printer::print_horizontal_line()
{
    *m_out_stream << "+";
    for (unsigned j = 0; j < m_column_widths.size(); ++j) {
        for (int i = 0; i < m_column_widths[j]; ++i) {
            *m_out_stream << "-";
        }
        *m_out_stream << "+";
    }
    *m_out_stream << "\n";
}

void table_printer::print_header()
{
    print_horizontal_line();
    *m_out_stream << "|";
    for (int i=0; i<get_num_columns(); ++i) {
        *m_out_stream << std::setw(m_column_widths.at(i)) <<
            m_column_headers.at(i).substr(0, m_column_widths.at(i));
        if (i != get_num_columns() - 1) {
            *m_out_stream << m_separator;
        }
    }
    *m_out_stream << "|\n";
    print_horizontal_line();
}

void table_printer::print_footer()
{
    print_horizontal_line();
}

table_printer& table_printer::operator<<(float input)
{
    output_decimal_number<float>(input);
    return *this;
}

table_printer& table_printer::operator<<(double input)
{
    output_decimal_number<double>(input);
    return *this;
}

}
