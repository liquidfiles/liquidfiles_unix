#include "table_printer.h"

#include <stdexcept>
#include <iomanip>
#include <stdexcept>

namespace lf {

table_printer::table_printer(std::ostream * output, const std::string & separator)
{
    out_stream_ = output;
    i_ = 0;
    j_ = 0;
    separator_ = separator;
    table_width_ = 0;
}

table_printer::~table_printer()
{
}

int table_printer::get_num_columns() const
{
    return column_headers_.size();
}

int table_printer::get_table_width() const
{
    return table_width_;
}

void table_printer::set_separator(const std::string &separator)
{
    separator_ = separator;
}

void table_printer::add_column(const std::string & header_name, int column_width)
{
    if (column_width < 4){
        throw std::invalid_argument("Column size has to be >= 4");
    }

    column_headers_.push_back(header_name);
    column_widths_.push_back(column_width);
    table_width_ += column_width + separator_.size(); // for the separator  
}

void table_printer::print_horizontal_line()
{
    *out_stream_ << "+"; // the left bar

    for (int i=0; i<table_width_-1; ++i)
        *out_stream_ << "-";

    *out_stream_ << "+"; // the right bar
    *out_stream_ << "\n";
}

void table_printer::print_header()
{
    print_horizontal_line();
    *out_stream_ << "|";

    for (int i=0; i<get_num_columns(); ++i){
        *out_stream_ << std::setw(column_widths_.at(i)) << column_headers_.at(i).substr(0, column_widths_.at(i));
        if (i != get_num_columns()-1){
            *out_stream_ << separator_;
        }
    }

    *out_stream_ << "|\n";
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
