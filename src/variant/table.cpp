#include "slw/variant/table.hpp"
#include "slw/utility.hpp"
#define LUA_COMPAT_APIINTCASTS 1
#include "lua.hpp"

////////////////////////////////////////////////////////////
/// table constructors
////////////////////////////////////////////////////////////
#include "variant/variant_ctors.hpp"
variant_ctors(slw::table)

// no need for derencing a table (what would that even do?!)

slw::table &slw::table::operator =(slw::table &value)
{
    value.M_ref.push();
    M_ref.assign();
    return *this;
}

bool slw::table::operator ==(table &rhs)
{
    auto m = slw::bind(
        [&]() { rhs.get().push(); get().push(); },
        [&]() { lua_pop(M_state.get(), 2); }
    );

    return lua_compare(M_state.get(), -1, -2, LUA_OPEQ);
}

bool slw::table::operator ==(table &&rhs)
{
    return *this == rhs;
}

bool slw::table::operator !=(table &rhs)
{
    auto m = slw::bind(
        [&]() { rhs.get().push(); get().push(); },
        [&]() { lua_pop(M_state.get(), 2); }
    );

    return !lua_compare(M_state.get(), -1, -2, LUA_OPEQ);
}

bool slw::table::operator !=(table &&rhs)
{
    return *this != rhs;
}

slw::reference slw::table::get()
{
    return M_ref;
}


slw::reference slw::table::set(const slw::string_t &k, slw::int_t idx)
{
    slw::reference ref { M_state, idx };

    get().push();
    ref.push();
    lua_setfield(M_state.get(), -2, k.c_str());
    lua_pop(M_state.get(), 1);
    return std::move(ref);
}
