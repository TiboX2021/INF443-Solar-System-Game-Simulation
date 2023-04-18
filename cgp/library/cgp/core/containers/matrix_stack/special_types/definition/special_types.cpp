
#include "special_types.hpp"
#include "cgp/core/base/base.hpp"

#include <cstdio>
#include <iostream>
#include <string>

namespace cgp
{
    std::string type_str(mat2 const &) { return "mat2"; }
    std::string type_str(mat3 const &) { return "mat3"; }
    std::string type_str(mat4 const &) { return "mat4"; }

}