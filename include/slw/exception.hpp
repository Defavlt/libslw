#ifndef SLW_EXCEPTION_HPP
#define SLW_EXCEPTION_HPP

#include <exception>
#include <stdexcept>
#include "slw/state.hpp"

namespace slw {

typedef std::out_of_range out_of_range;

struct no_type_specialization : public std::logic_error {
    explicit no_type_specialization();
};

struct unexpected_type : public std::runtime_error {
    explicit unexpected_type(shared_state);
};

struct memory_allocation_failed : public std::runtime_error{
    explicit memory_allocation_failed();
};

struct error_error : public std::runtime_error {
    explicit error_error();
};

struct unexpected_argument_range : public std::runtime_error {
    explicit unexpected_argument_range(shared_state, slw::size_t got, slw::size_t expected);
};

}/*ns slw*/

#endif/*SLW_EXCEPTION_HPP*/
