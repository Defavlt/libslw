#ifndef SLW_UTILITY_HPP
#define SLW_UTILITY_HPP

#include <iostream>
#include "slw/state.h"

namespace slw {
namespace debug {
void print_stack_contents(slw::State &, std::ostream &);
}
}

#endif//SLW_UTILITY_HPP
