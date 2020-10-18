#include "slw/variant/factory.hpp"
#define LUA_COMPAT_APIINTCASTS 1
#include "lua.hpp"

#include <iostream>

template<> slw::string slw::make_variant<slw::string_t>(slw::shared_state &state, const slw::string_t &v)
{
    lua_pushstring(state.get(), v.c_str());
    return slw::string {
        state
    };;
}

slw::boolean slw::make_variant(slw::shared_state &state, slw::bool_t v)
{
    lua_pushboolean(state.get(), v);
    return slw::boolean {
        state
    };
}

slw::integer slw::make_variant(slw::shared_state &state, slw::int_t v)
{
    lua_pushinteger(state.get(), v);
    return slw::integer {
        state
    };
}

slw::number slw::make_variant(slw::shared_state &state, slw::number_t v)
{
    lua_pushnumber(state.get(), v);
    return slw::number {
        state
    };
}

slw::table slw::make_variant(slw::shared_state &state)
{
    lua_createtable(state.get(), 0, 0);
    return slw::table {
        state
    };
}

slw::uinteger slw::make_variant(slw::shared_state &state, slw::uint_t v)
{
    lua_pushunsigned(state.get(), v);
    return slw::uinteger {
        state
    };;
}
