#ifndef SLW_UTILITY_HPP
#define SLW_UTILITY_HPP

#include <iostream>
#include "slw/state.h"

namespace slw {
namespace debug {
void print_stack_contents(slw::State &, std::ostream &);

inline void print_stack_contents(slw::State &state)
{
    print_stack_contents(state, std::cout);
}

} //debug
} //slw

#endif//SLW_UTILITY_HPP
