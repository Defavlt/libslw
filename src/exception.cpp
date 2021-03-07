#include "slw/exception.hpp"
#include "lua.hpp"

slw::no_type_specialization::no_type_specialization()
    : std::logic_error("missing, or no defined type template specialization")
{
}

slw::unexpected_type::unexpected_type(shared_state  state)
    : std::runtime_error("unexpected type encountered")
{ luaL_error(state.get(), " unexpected type encountered");
}

slw::memory_allocation_failed::memory_allocation_failed()
    : std::runtime_error("memory allocation failed")
{
}

slw::error_error::error_error()
    : std::runtime_error("error while handling an error")
{
}

slw::unexpected_argument_range::unexpected_argument_range(
        shared_state state
        , slw::size_t got
        , slw::size_t expected)
    : std::runtime_error("expected "
                         + std::to_string(expected)
                         + ", got "
                         + std::to_string(got))
{ luaL_error(state.get(), (" expected "
             + std::to_string(expected)
             + " arguments, got "
             + std::to_string(got)).c_str());
}

slw::unexpected_argument_type::unexpected_argument_type(
        slw::shared_state state,
        slw::type_e got,
        slw::type_e expected)
    : std::runtime_error("expected"
                         + std::to_string(expected)
                         + ", got "
                         + std::to_string(got))
{ luaL_error(state.get(), (" expected "
            + std::to_string(expected)
            + ", got "
            + std::to_string(got)).c_str());
}
