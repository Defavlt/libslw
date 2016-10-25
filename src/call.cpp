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
#include "slw/types.h"

#include "slw/get_field.h"
#include "slw/utility.h"

#include <lua.hpp>

slw::Call::Call(slw::State &state, slw::string_t fn)
    : state(state)
    , call_ref(slw::get_field(state.state, fn.c_str())
               ? luaL_ref(state.state, slw::internal::indexes::registry)
               : 0)
    , isValid(call_ref)
    , args(0)
{
}

slw::Call::Call(slw::State &state, slw::string_t name, slw::entry_t callback, void *user)
    : state(state)
    , call_ref(0)
    , isValid(false)
    , args(0)
{
    lua_pushlightuserdata(state.state, this);
    state.push((int) entries.size());

    lua_pushcclosure(state.state, &handler, 2);
    lua_setglobal(state.state, name.c_str());

    entry_data_t entry (callback, name, *this, user);
    entries.push_back(entry);

    if (slw::get_field(state, name.c_str()))
    {
        isValid = true;
        call_ref = luaL_ref(state.state, slw::internal::indexes::registry);
    }
}

slw::Call::~Call()
{
    for (entry_data_t &entry: entries) {
        state.push();
        lua_setglobal(state.state, entry.event.c_str());
    }

    luaL_unref(state.state, LUA_REGISTRYINDEX, call_ref);
}

void slw::Call::clear(void)
{
    lua_pop(state.state, args);
    args = 0;
}


#define PARAM(TYPE)\
void slw::Call::param(TYPE value) {\
    state.push(value);\
    ++args;\
}

PARAM(slw::string_t)
PARAM(slw::number_t)
PARAM(long)
PARAM(int)
PARAM(char)
PARAM(bool)

#undef PARAM

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

int slw::Call::handler(lua_State *ptr_state)
{
    slw::State state(ptr_state);
    slw::Call &call = *(slw::Call *)lua_touserdata(ptr_state, lua_upvalueindex(1));
    int entry_i = 0;

    if (!state.peek(entry_i, slw::internal::indexes::upvalue(2)))
        return 0;

    slw::Call::entry_data_t &entry = call.entries[entry_i];

    return (*entry.entry)(state, entry.user);
}

slw::Call::entry_data_t::entry_data_t(const entry_data_t &rhs)
    : entry(rhs.entry)
    , event(rhs.event)
    , call(rhs.call)
    , user(rhs.user)
{
}

slw::Call::entry_data_t::entry_data_t(slw::entry_t entry, slw::string_t event, slw::Call &call, void *user)
    : entry(entry)
    , event(event)
    , call(call)
    , user(user)
{

}
