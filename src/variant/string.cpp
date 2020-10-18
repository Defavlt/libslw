#include "slw/variant/string.hpp"
#include "lua.hpp"

////////////////////////////////////////////////////////////
/// string constructors
////////////////////////////////////////////////////////////
#include "variant/variant_ctors.hpp"
variant_ctors(slw::string)

////////////////////////////////////////////////////////////
/// string equality operators
////////////////////////////////////////////////////////////
#include "variant/variant_op_eq.hpp"
variant_op_eq(slw::string, ==)
variant_op_eq(slw::string, !=)

slw::string::expected_value_type slw::string::operator *()
{
    M_ref.push();
    expected_value_type r = luaL_tolstring(M_state.get(), -1, nullptr);
    // Return, *and* push onto the stack?
    // The Lua C API string handling routines are off!
    lua_pop(M_state.get(), 2);
    return r;
}

slw::string::expected_value_type slw::string::operator *() const
{
    // This is ugly, but there's no other way to poll data
    // from the state, while keeping const correctness.
    slw::string &ref = *(const_cast<slw::string *>(this));
    return *ref;
}

slw::string &slw::string::operator =(const slw::string::expected_value_type &value)
{
    lua_pushstring(M_state.get(), value.c_str());
    M_ref.assign();
    return *this;
}

char slw::string::operator [](slw::int_t i)
{
    slw::string_t str = *(*this);
    slw::int_t abs_i = std::abs(i);

    if (abs_i > str.length()) {
        throw slw::out_of_range {
            "std::abs(" + std::to_string(i) + ") >"
            + std::to_string(str.length())
        };
    }

    if (i < 0)
        i = str.length() - i;

    return str[i];
}

slw::reference slw::string::get()
{
    return M_ref;
}
