#include "slw/functional.hpp"

#include <string>
#include <iostream>

int raw_call(lua_State *L)
{
    slw::callable *C = (slw::callable *)lua_touserdata(L, lua_upvalueindex(1));
    return (*C)();
}

slw::reference slw::make_callable(slw::shared_state state, slw::table t, const slw::string_t &k, callable &f)
{
    lua_pushlightuserdata(state.get(), &f);
    lua_pushcclosure(state.get(), raw_call, 1);
    t.set(k, -1);
    return t[k];
}
