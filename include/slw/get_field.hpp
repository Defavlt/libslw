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

#ifndef SLW_GET_FIELD_H
#define SLW_GET_FIELD_H

#include "slw/state.hpp"
#include "slw/types.hpp"

struct lua_State;


namespace slw
{
/* \brief get a field or object from a Lua state
 * \return true if object or function was found
 */
bool get_field(lua_State *state, slw::string_t);
bool get_field(slw::State &, slw::string_t);
}

#endif//SLW_GET_FIELD_H
