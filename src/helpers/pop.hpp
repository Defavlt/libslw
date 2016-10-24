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

#ifndef SLW_HELPERS_POP_HPP
#define SLW_HELPERS_POP_HPP

#include <exception>
#include <functional>
#include <lua.hpp>

#include "slw/state.h"
#include "slw/types.h"
#include "tostring.hpp"

namespace slw {
namespace internal {
    template<
        typename _value_t
        , int _type_t
        , typename _converter_t = _value_t(lua_State *, int)>
    inline bool pop(lua_State *state, _value_t &value, bool force, _converter_t converter) {
        const int top = lua_gettop(state);
        const int offset = -1;
        const int type = lua_type(state, offset);

        if (!top || type != _type_t) {
            if (force)
                lua_pop(state, 1);
            return false;
        }

        value = converter(state, offset);
        lua_pop(state, 1);
        return true;
    }

}// namespace internal

template<typename _value_t>
inline bool pop(lua_State *, _value_t &, bool) {
    throw std::runtime_error("inaccessible or missing type specialization");
}

template<>
inline bool pop<slw::string_t>(lua_State *state, string_t &value, bool force) {
    return slw::internal::pop<slw::string_t, slw::TSTRING>(state, value, force, slw::internal::tostring);
}

template<>
inline bool pop<slw::number_t>(lua_State *state, slw::number_t &value, bool force) {
    return slw::internal::pop<slw::number_t, slw::TNUMBER>(state, value, force, lua_tonumber);
}

template<>
inline bool pop<long>(lua_State *state, long &value, bool force) {
    return slw::internal::pop<long, slw::TNUMBER>(state, value, force, lua_tonumber);
}

template<>
inline bool pop<int>(lua_State *state, int &value, bool force) {
    return slw::internal::pop<int, slw::TNUMBER>(state, value, force, lua_tonumber);
}

template<>
inline bool pop<char>(lua_State *state, char &value, bool force) {
    return slw::internal::pop<char, slw::TNUMBER>(state, value, force, lua_tonumber);
}

template<>
inline bool pop<bool>(lua_State *state, bool &value, bool force) {
    return slw::internal::pop<bool, slw::TBOOLEAN>(state, value, force, lua_toboolean);
}

inline bool pop(lua_State *state) {
    lua_pop(state, 1);
    return true;
}
}// namespace slw

#endif//SLW_HELPERS_POP_HPP
