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

#include <lua.hpp>

#include "slw/state.h"
#include "slw/field.h"
#include "helpers/push.hpp"
#include "helpers/peek.hpp"

const char *const slw::State::magic = "__slw__state__magic";

int slw::State::ref()
{
    slw::Field field(*this);

    auto ref_count = field.field<int>(magic, 0);
    ref_count = ref_count + 1;

    return ref_count;
}

int slw::State::deref()
{
    slw::Field field(*this);

    auto ref_count = field.field<int>(magic, 0);
    ref_count = ref_count - 1;

    return ref_count;
}

int slw::State::refs()
{
    slw::Field field(*this);

    auto ref_count = field.field<int>(magic, 0);

    return ref_count;
}

slw::State::State()
    : state(luaL_newstate())
{
    luaL_openlibs(state);

    ref();
}

slw::State::State(slw::State &state)
    : state(state.state)
{
    ref();
}

slw::State::State(lua_State *state)
    : state(state)
{
    ref();
}

slw::State::~State()
{
    if (!deref())
        lua_close(state);

    state = NULL;
}

bool
slw::State::load(const char *str, const bool isFile /* = true */)
{
    if (isFile)
        luaL_loadfile(state, str);

    else
        luaL_loadstring(state, str);

    return lua_pcall(state, 0, LUA_MULTRET, 0);
}

void slw::State::dostring(slw::string_t str)
{
    luaL_dostring(state, str.c_str());
}

int slw::State::size()
{
    return lua_gettop(state);
}

int slw::State::type(int index/* = -1 */)
{
    return lua_type(state, index);
}

slw::string_t slw::State::type_name(int type)
{
    return lua_typename(state, type);
}

int slw::State::top()
{
    return lua_gettop(state);
}
