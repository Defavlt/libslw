/**
* Copyright (C) 2016- The Authors
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*	Authors:
*	Marcus Hansson <marcus.hansson@email.com>
*	Andre Andersson <andre.eric.andersson@gmail.com>
**/

#include "slw/call.h"
#include "slw/state.h"

#include "slw/get_field.h"

#include <lua.hpp>

slw::Call::Call(slw::State &state, slw::string_t fn)
    : call_ref(slw::get_field(state.state, fn.c_str())
               ? luaL_ref(state.state, LUA_REGISTRYINDEX)
               : 0)
    , args(0)
    , state(state) {}

slw::Call::Call(slw::State &state, int index)
    : call_ref(0), args(0), state(state), isValid(false)
{
    lua_pushvalue(state.state, index);

    call_ref = luaL_ref(state.state, LUA_REGISTRYINDEX);
}

slw::Call::~Call()
{
    luaL_unref(state.state, LUA_REGISTRYINDEX, call_ref);
}

void slw::Call::clear(void)
{
    lua_pop(state.state, args);
    args = 0;
}

void slw::Call::param()
{
    state.push();
    ++args;
}

bool slw::Call::call(const unsigned int nresults /*= 0*/)
{
    if (call_ref == 0)
        return false;

#if defined(LUA_AS_CPP)
    try {
#endif
        const int fn_index = -((int)(args)) - 1;

        if (state.type(fn_index) != LUA_TFUNCTION) {
            lua_rawgeti(state.state, LUA_REGISTRYINDEX, call_ref);
            lua_insert(state.state, fn_index);
        }

        if (lua_pcall(state.state, args, nresults, 0)) {
            const char *error = lua_tostring(state.state, -1);

            log_error("%s", error);
            lua_pop(state.state, 1);

            return false;
        }

#if defined(LUA_AS_CPP)
    } catch (std::exception *e) {
        throw e;
    }
#endif

    return true;
}
