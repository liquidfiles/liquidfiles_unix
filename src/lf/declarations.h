#pragma once

#include <iostream>

namespace lf {

enum class report_level {
    silent,
    normal,
    verbose
};

enum class validate_cert {
    validate,
    not_validate
};

inline std::ostream& operator<<(std::ostream& s, validate_cert v)
{
    s << static_cast<int>(v);
    return s;
}

enum class output_format {
    table,
    csv
};

}
