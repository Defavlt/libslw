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

#include "slw/get_field.hpp"
#include "lua.hpp"
#include <string>
#include <vector>
#include <iostream>

#define _head(x) std::get<0>( (x) )
#define _tail(x) std::get<1>( (x) )

slw::internal::cons slw::internal::split_path(const std::string &path, const char separator/* = '.'*/)
{
    std::string h { "" };
    std::size_t i = 0;

    for (; i < path.length(); ++i) {
        const char c = path[i];
        if (separator == c) {
             // break the collection if, and only if, h isn't empty
             //  since an empty h is an indication of multiple separators
            if (h.length())
                break;
             // make sure we don't collect the character (it's a separator)
             continue;
        }

        h += c;
    }

    std::string t { "" };

    // get the remaining string *after* the separator/s,
    //  but if the index is less than the path length,
    //  the remainder is an *empty string*
    if (++i < path.length())
        t = path.substr(i);

    return std::make_tuple(
        h,
        t
    );
}

slw::internal::path::iterator::iterator(const std::string &path, const char separator/* = '.'*/)
    : M_cons(slw::internal::split_path(path, separator))
    , M_separator(separator)
    , M_end(!path.length())
{
}

slw::internal::path::iterator &slw::internal::path::iterator::operator ++()
{
    M_cons = split_path(tail(), M_separator);
    M_end = !tail().length() && !_head(M_cons).length();
    return *this;
}

slw::internal::path::iterator slw::internal::path::iterator::operator ++(int)
{
    path::iterator at = *this;
    ++(*this);
    return at;
}

bool slw::internal::path::iterator::operator ==(const slw::internal::path::iterator &other) const
{
    return this == &other
            || (_head(M_cons) == _head(M_cons)
                && _tail(M_cons) == _tail(other.M_cons)
                && M_end == other.M_end);
}

bool slw::internal::path::iterator::operator !=(const path::iterator &other) const
{
    return !(*this == other);
}

slw::internal::path::iterator::reference slw::internal::path::iterator::operator *()
{
    return _head(M_cons);
}

slw::internal::path::iterator::reference slw::internal::path::iterator::tail()
{
    return _tail(M_cons);
}

slw::internal::path::path(const std::string &path, const char separator/* = '.'*/)
    : M_end("", separator)
    , M_begin(path, separator)
{
}

slw::internal::path::iterator slw::internal::path::begin()
{
    return M_begin;
}

slw::internal::path::iterator slw::internal::path::end()
{
    return M_end;
}

void slw::internal::touch_path(lua_State *L, const std::string &path, int t, char separator)
{
    slw::internal::path to { path, separator };
    slw::internal::path::iterator at = to.begin();

    // make sure we catch any attempt to index global fields early on
    // LUA_GLOBALSINDEX, et. al. doesn't play very well with pseudo-indices
    if (LUA_RIDX_GLOBALS == t) {
        lua_getglobal(L, (*at).c_str());
        ++at;
    }

    int top = lua_gettop(L);
    int type = lua_type(L, -1);

    if (to.end() != at) {

        if (!lua_istable(L, -1)) {
            lua_pop(L, 1);
            lua_createtable(L, 0, 1);
            lua_setglobal(L, (*at).c_str());
            lua_getglobal(L, (*at).c_str());
        }

        lua_getfield(L, -1, (*at).c_str());

        for (int t = -1, i = lua_gettop(L); i > 0; --i, --t) {
            top = lua_gettop(L);
            type = lua_type(L, -1);
        }

        lua_replace(L, -2);
        ++at;

        slw::internal::path::iterator next = at;
        ++next;
        for (; to.end() != at; ++at, ++next) {
            lua_getfield(L, -1, (*at).c_str());

            if (to.end() != next && !lua_istable(L, -1)) {
                lua_createtable(L, 0, 1);
                lua_setfield(L, -2, (*at).c_str());
                lua_getfield(L, -1, (*at).c_str());
            }

            lua_replace(L, -2);
        }
    }
}
