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

#ifndef SLW_TYPES_H
#define SLW_TYPES_H

#include <string>
#include <cstdlib>
#include <stddef.h>
#include <lua.hpp>

namespace slw {

typedef double number_t;
typedef unsigned int uint_t;
typedef int int_t;
typedef std::string string_t;
typedef std::size_t size_t;
typedef bool bool_t;
typedef int reference_t;

struct table_t {
};

struct function_t {
};

enum type_e
{
    TNONE = LUA_TNONE,
    TNIL = LUA_TNIL, //LUA_TNIL
    TBOOLEAN = LUA_TBOOLEAN, //LUA_TBOOLEAN
    TLIGHTUSERDATA = LUA_TLIGHTUSERDATA, //LUA_TLIGHTUSERDATA
    TNUMBER = LUA_TNUMBER, //LUA_TNUMBER
    TSTRING = LUA_TSTRING, //LUA_TSTRING
    TTABLE = LUA_TTABLE, //LUA_TTABLE
    TFUNCTION = LUA_TFUNCTION, //LUA_TFUNCTION
    TUSERDATA = LUA_TUSERDATA, //LUA_TUSERDATA
    TTHREAD = LUA_TTHREAD, //LUA_TTHREAD
};

namespace internal {
namespace indexes {
    extern const slw::int_t globals;
    extern const slw::int_t registry;

    int upvalue(int offset = 0);
}//ns indexes
}//ns internal
}//ns slw

#endif//SLW_TYPES_H
