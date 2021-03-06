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

#include "lua.hpp"
#include "slw/state.hpp"

slw::shared_state slw::make_state()
{
    lua_State *state = luaL_newstate();
    return shared_state (state, lua_close);
}

void slw::open_libs(slw::shared_state state)
{
    luaL_openlibs(state.get());
}

slw::size_t slw::get_size(const slw::shared_state &state)
{
    return lua_gettop(state.get());
}

void slw::clear(slw::shared_state &state, slw::int_t N/* = 0*/)
{
    if (!N)
        N = slw::get_size(state);

    lua_pop(state.get(), N);
}
