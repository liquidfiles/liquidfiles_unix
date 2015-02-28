#pragma once

#include "messenger.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

namespace lf {

class table_printer
{
public:
    table_printer(std::ostream * output, const std::string & separator = "|");
    ~table_printer();

    int get_num_columns() const;
    int get_table_width() const;
    void set_separator(const std::string & separator);

    void add_column(const std::string & header_name, int column_width);
    void print_header();
    void print_footer();

    table_printer& operator<<(endl_type input){
        while (j_ != 0){
            *this << "";
        }
        return *this;
    }

    table_printer& operator<<(float input);
    table_printer& operator<<(double input);

    template<typename T>
    table_printer& operator<<(T input)
    {
        if (j_ == 0) {
            *out_stream_ << "|";
        }

        *out_stream_ << std::setw(column_widths_.at(j_))
            << input;

        if (j_ == get_num_columns()-1){
            *out_stream_ << "|\n";
            i_ = i_ + 1;
            j_ = 0;
        } else {
            *out_stream_ << separator_;
            j_ = j_ + 1;
        }

        return *this;
    }

private:
    void print_horizontal_line();

    template<typename T>
    void output_decimal_number(T input);

    std::ostream * out_stream_;
    std::vector<std::string> column_headers_;
    std::vector<int> column_widths_;
    std::string separator_;

    int i_;
    int j_;

    int table_width_;
};

template<typename T>
void table_printer::output_decimal_number(T input)
{
    if (input < 10*(column_widths_.at(j_)-1) || input > 10*column_widths_.at(j_)){
        std::stringstream string_out;
        string_out << std::setiosflags(std::ios::fixed)
            << std::setprecision(column_widths_.at(j_))
            << std::setw(column_widths_.at(j_))
            << input;

        std::string string_rep_of_number = string_out.str();

        string_rep_of_number[column_widths_.at(j_)-1] = '*';
        std::string string_to_print = string_rep_of_number.substr(0, column_widths_.at(j_));
        *out_stream_ << string_to_print;
    } else {

        int precision = column_widths_.at(j_) - 1; // leave room for the decimal point
        if (input < 0)
            --precision;

        if (input < -1 || input > 1){
            int num_digits_before_decimal = 1 + (int)log10(std::abs(input));
            precision -= num_digits_before_decimal;
        }
        else
            precision --; // e.g. 0.12345 or -0.1234

        if (precision < 0)
            precision = 0; // don't go negative with precision

        *out_stream_ << std::setiosflags(std::ios::fixed)
            << std::setprecision(precision)
            << std::setw(column_widths_.at(j_))
            << input;
    }

    if (j_ == get_num_columns()-1){
        *out_stream_ << "|\n";
        i_ = i_ + 1;
        j_ = 0;
    } else {
        *out_stream_ << separator_;
        j_ = j_ + 1;
    }
}

}
