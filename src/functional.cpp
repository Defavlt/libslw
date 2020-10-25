#include "slw/functional.hpp"

#include <string>
#include <iostream>

int raw_call(lua_State *L)
{
    slw::callable *C = (slw::callable *)lua_touserdata(L, lua_upvalueindex(1));
    return (*C)();
}

slw::reference slw::make_callable(slw::shared_state state, callable &f)
{
    lua_pushlightuserdata(state.get(), &f);
    lua_pushcclosure(state.get(), raw_call, 1);
    return slw::reference {
        state
    };
}
