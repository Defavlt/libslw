#include "slw/utility.hpp"
#include "lua.hpp"
#include <string>

#define is_lua_type(native_type, lua_type_check)            \
bool slw::is(slw::shared_state state, native_type &value)   \
{                                                           \
    auto m = bind(                                          \
        [&]() { value.get().push(); },                      \
        [&]() { lua_pop(state.get(), 1); }                  \
    );                                                      \
                                                            \
    return lua_type_check(state.get(), -1);                 \
}

is_lua_type(slw::number, lua_isnumber)
is_lua_type(slw::uinteger, lua_isnumber)
is_lua_type(slw::integer, lua_isnumber)
is_lua_type(slw::string, lua_isstring)
is_lua_type(slw::boolean, lua_isboolean)

bool slw::is(slw::shared_state state, slw::table &value)
{
    auto m = bind(
        [&]() { value.get().push(); },
        [&]() { lua_pop(state.get(), 1); }
    );

    return lua_istable(state.get(), -1);
}

#undef is_lua_type

slw::table slw::registry(slw::shared_state state)
{
    return slw::table {
        state, slw::internal::indexes::registry
    };
}

slw::table slw::globals(slw::shared_state state)
{
    return registry(state).get<slw::table_t>(slw::internal::indexes::globals);
}
