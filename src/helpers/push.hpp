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

#ifndef SLW_HELPERS_PUSH_HPP
#define SLW_HELPERS_PUSH_HPP

#include <exception>
#include <functional>
#include <lua.hpp>

#include "slw/state.h"
#include "slw/types.h"

namespace slw {
namespace internal {
    template<typename _num_t>
    inline void push(lua_State *state, _num_t num) {
        lua_pushnumber(state, num);
    }
}

template<typename _value_t>
inline void push(lua_State *, _value_t) {
    throw std::runtime_error("inaccessible or missing type specialization");
}

template<>
inline void push<string_t>(lua_State *state, string_t value) {
    lua_pushstring(state, value.c_str());
}

template<>
inline void push<slw::number_t>(lua_State *state, slw::number_t value) {
    slw::internal::push(state, value);
}

template<>
inline void push<long>(lua_State *state, long value) {
    slw::internal::push(state, value);
}

template<>
inline void push<int>(lua_State *state, int value) {
    slw::internal::push(state, value);
}

template<>
inline void push<char>(lua_State *state, char value) {
    slw::internal::push(state, value);
}

template<>
inline void push<bool>(lua_State *state, bool value) {
    lua_pushboolean(state, value);
}

inline void push(lua_State *state) {
    lua_pushnil(state);
}
}// namespace slw


#endif//SLW_HELPERS_PUSH_HPP
