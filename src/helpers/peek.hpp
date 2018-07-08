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

#ifndef SLW_HELPERS_PEEK_HPP
#define SLW_HELPERS_PEEK_HPP

#include <exception>
#include <functional>
#include "lua.hpp"

#include "slw/state.hpp"
#include "slw/types.hpp"
#include "tostring.hpp"

namespace slw {
namespace internal {

    template<
        typename _value_t
        , int _type_t
        , typename _converter_t = _value_t(lua_State *, int)>
    inline bool peek(lua_State *state, _value_t &value, int offset, _converter_t converter) {
        const int top = lua_gettop(state);
        if (!top || lua_type(state, offset) != _type_t) {
            return false;
        }

        value = converter(state, offset);
        return true;
    }

}// namespace internal

template<typename _value_t>
inline bool peek(lua_State *, _value_t &, int) {
    throw std::runtime_error("inaccessible or missing type specialization");
}

template<>
inline bool peek<slw::string_t>(lua_State *state, string_t &value, int offset) {
    return slw::internal::peek<slw::string_t, slw::TSTRING>(state, value, offset, slw::internal::tostring);
}

template<>
inline bool peek<slw::number_t>(lua_State *state, slw::number_t &value, int offset) {
    return slw::internal::peek<slw::number_t, slw::TNUMBER>(state, value, offset, lua_tonumber);
}

template<>
inline bool peek<long>(lua_State *state, long &value, int offset) {
    return slw::internal::peek<long, slw::TNUMBER>(state, value, offset, lua_tonumber);
}

template<>
inline bool peek<int>(lua_State *state, int &value, int offset) {
    return slw::internal::peek<int, slw::TNUMBER>(state, value, offset, lua_tonumber);
}

template<>
inline bool peek<char>(lua_State *state, char &value, int offset) {
    return slw::internal::peek<char, slw::TNUMBER>(state, value, offset, lua_tonumber);
}

template<>
inline bool peek<bool>(lua_State *state, bool &value, int offset) {
    return slw::internal::peek<bool, slw::TBOOLEAN>(state, value, offset, lua_toboolean);
}
}// namespace slw

#endif//SLW_HELPERS_PEEK_HPP
