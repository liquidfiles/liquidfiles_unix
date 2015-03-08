#pragma once

#include "messenger.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace lf {

/**
 * @class table_printer.
 * @brief Console pretty table printer class.
 */
class table_printer
{
public:
    /**
     * @brief Constructor.
     * @param output Stream to output the table.
     * @param separator Columns separator string.
     */
    table_printer(std::ostream* output, const std::string& separator = "|");

    /// @brief Gets the number of columns.
    int get_num_columns() const;

    /**
     * @brief adds the column.
     * @brief header_name Name of header of the column.
     * @brief column_width Width of column.
     */
    void add_column(const std::string& header_name, int column_width);

    /// @brief Prints header to the stream.
    void print_header();

    /// @brief Prints footer to the stream.
    void print_footer();

    /// @brief Prints endline to the stream.
    table_printer& operator<<(endl_type input){
        while (m_j != 0) {
            *this << "";
        }
        return *this;
    }

    /// @brief Prints float number to the stream.
    table_printer& operator<<(float input);

    /// @brief Prints double number to the stream.
    table_printer& operator<<(double input);

    /**
     * @brief Prints the given input to the stream.
     * @tparam T type of input.
     * @param input Input to print to the stream.
     */
    template<typename T>
    table_printer& operator<<(T input)
    {
        if (m_j == 0) {
            *m_out_stream << "|";
        }

        *m_out_stream << std::setw(m_column_widths.at(m_j))
            << input;

        if (m_j == get_num_columns()-1) {
            *m_out_stream << "|\n";
            m_i = m_i + 1;
            m_j = 0;
        } else {
            *m_out_stream << m_separator;
            m_j = m_j + 1;
        }

        return *this;
    }

private:
    void print_horizontal_line();

    template<typename T>
    void output_decimal_number(T input);

    std::ostream* m_out_stream;
    std::vector<std::string> m_column_headers;
    std::vector<int> m_column_widths;
    std::string m_separator;

    int m_i;
    int m_j;
};

template<typename T>
void table_printer::output_decimal_number(T input)
{
    if (input < 10 * (m_column_widths.at(m_j) - 1) || input > 10 * m_column_widths.at(m_j)) {
        std::stringstream string_out;
        string_out << std::setiosflags(std::ios::fixed)
            << std::setprecision(m_column_widths.at(m_j))
            << std::setw(m_column_widths.at(m_j))
            << input;
        std::string string_rep_of_number = string_out.str();
        string_rep_of_number[m_column_widths.at(m_j) - 1] = '*';
        std::string string_to_print = string_rep_of_number.substr(0, m_column_widths.at(m_j));
        *m_out_stream << string_to_print;
    } else {
        int precision = m_column_widths.at(m_j) - 1; // leave room for the decimal point
        if (input < 0) {
            --precision;
        }

        if (input < -1 || input > 1) {
            int num_digits_before_decimal = 1 + (int)log10(std::abs(input));
            precision -= num_digits_before_decimal;
        } else {
            --precision;
        }

        if (precision < 0) {
            precision = 0; // don't go negative with precision
        }

        *m_out_stream << std::setiosflags(std::ios::fixed)
            << std::setprecision(precision)
            << std::setw(m_column_widths.at(m_j))
            << input;
    }

    if (m_j == get_num_columns() - 1) {
        *m_out_stream << "|\n";
        m_i = m_i + 1;
        m_j = 0;
    } else {
        *m_out_stream << m_separator;
        m_j = m_j + 1;
    }
}

}
