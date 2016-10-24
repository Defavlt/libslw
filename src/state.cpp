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
#include "helpers/push.hpp"
#include "helpers/peek.hpp"
#include "helpers/pop.hpp"

slw::State::State()
    : state(luaL_newstate())
{
    luaL_openlibs(state);

}

slw::State::State(slw::State &state)
    : state(state.state)
{
}

slw::State::State(lua_State *state)
    : state(state)
{
}

slw::State::~State()
{
    lua_close(state);
    state = NULL;
}

bool
slw::State::load(const char *str, const bool str_is_file /* = true */)
{
    if ( str_is_file )
        luaL_loadfile( state, str );

    else
        luaL_loadstring( state, str );

    return lua_pcall( state, 0, LUA_MULTRET, 0 );
}

void
slw::State::registerfn( const char *event, slw::entry_t callback, void *user /* = NULL */ )
{
    slw::State::entry_data_t *point = new slw::State::entry_data_t;

    point->entry = callback;
    point->event = event;
    point->state = this;
    point->user	 = user;

    lua_pushlightuserdata(state, point);
    lua_pushcclosure(state, &handler, 1);
    lua_setglobal(state, event);
}

bool slw::State::pop(slw::string_t &value, bool force)
{
    return slw::pop(state, value, force);
}

bool slw::State::pop(number_t &value, bool force)
{
    return slw::pop(state, value, force);
}

bool slw::State::pop(long &value, bool force)
{
    return slw::pop(state, value, force);
}

bool slw::State::pop(int &value, bool force)
{
    return slw::pop(state, value, force);
}

bool slw::State::pop(char &value, bool force)
{
    return slw::pop(state, value, force);
}

bool slw::State::pop(bool &value, bool force)
{
    return slw::pop(state, value, force);
}

bool slw::State::pop()
{
    return slw::pop(state);
}

bool slw::State::peek(slw::string_t &value, int offset)
{
    return slw::peek(state, value, offset);
}

bool slw::State::peek(number_t &value, int offset)
{
    return slw::peek(state, value, offset);
}

bool slw::State::peek(long &value, int offset)
{
    return slw::peek(state, value, offset);
}

bool slw::State::peek(int &value, int offset)
{
    return slw::peek(state, value, offset);
}

bool slw::State::peek(char &value, int offset)
{
    return slw::peek(state, value, offset);
}

bool slw::State::peek(bool &value, int offset)
{
    return slw::peek(state, value, offset);
}

void slw::State::push(slw::string_t value)
{
    slw::push(state, value);
}

void slw::State::push(number_t value)
{
    slw::push(state, value);
}

void slw::State::push(long value)
{
    slw::push(state, value);
}

void slw::State::push(int value)
{
    slw::push(state, value);
}

void slw::State::push(char value)
{
    slw::push(state, value);
}

void slw::State::push(bool value)
{
    slw::push(state, value);
}

void slw::State::push()
{
    slw::push(state);
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

int slw::State::handler(lua_State *state)
{
    slw::State::entry_data_t *point =
        (slw::State::entry_data_t *)
        (lua_touserdata(state, lua_upvalueindex(1)));

    return (*point->entry)(*point->state, point->user);
}
