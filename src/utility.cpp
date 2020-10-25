#include "slw/utility.hpp"
#include "lua.hpp"
#include <string>

#define is_lua_type(native_type, lua_type_check)                        \
template<>                                                              \
bool slw::is<native_type>(slw::reference &r)   \
{                                                                       \
    auto m = bind(                                                      \
        [&]() { r.push(); },                                            \
        [&]() { lua_pop(r.get_state().get(), 1); }                              \
    );                                                                  \
                                                                        \
    return lua_type_check(r.get_state().get(), -1);                     \
}

is_lua_type(slw::number_t, lua_isnumber)
is_lua_type(slw::uint_t, lua_isnumber)
is_lua_type(slw::int_t, lua_isnumber)
is_lua_type(slw::string_t, lua_isstring)
is_lua_type(slw::bool_t, lua_isboolean)
is_lua_type(slw::table_t, lua_istable)

#undef is_lua_type

#define as_lua_type(native_type, lua_to_type)                               \
template<>                                                                  \
native_type slw::as<native_type>(slw::reference &r)\
{                                                                           \
    auto m = bind(                                                          \
        [&]() { r.push(); },                                                \
        [&]() { lua_pop(r.get_state().get(), 1); }                                  \
    );                                                                      \
                                                                            \
    return lua_to_type(r.get_state().get(), -1);                            \
}

as_lua_type(slw::number_t, lua_tonumber)
as_lua_type(slw::uint_t, lua_tonumber)
as_lua_type(slw::int_t, lua_tonumber)
as_lua_type(slw::string_t, lua_tostring)
as_lua_type(slw::bool_t, lua_toboolean)

template<>
// for convenience and completeness sake only
slw::reference slw::as<slw::reference>(slw::reference &r)
{ return r;
}

#undef as_lua_type

slw::reference slw::registry(slw::shared_state state)
{
    return slw::reference {
        state, slw::internal::indexes::registry
    };
}

slw::reference slw::globals(slw::shared_state state)
{
    return registry(state)[slw::internal::indexes::globals];
}
