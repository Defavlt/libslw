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

#include "slw/types.hpp"
#include "lua.hpp"

const int slw::internal::indexes::globals(LUA_GLOBALSINDEX);
const int slw::internal::indexes::registry(LUA_REGISTRYINDEX);
const int slw::internal::indexes::environ(LUA_ENVIRONINDEX);

int slw::internal::indexes::upvalue(int offset)
{
    return lua_upvalueindex(offset);
}
