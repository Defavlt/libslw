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

namespace slw {

typedef double number_t;
typedef unsigned int uint_t;
typedef int int_t;
typedef std::string string_t;
typedef std::size_t size_t;
typedef bool bool_t;

struct table_t {
};

struct function_t {
};

enum type_e
{
    TNONE = -1, //LUA_TNONE
    TNIL = 0, //LUA_TNIL
    TBOOLEAN = 1, //LUA_TBOOLEAN
    TLIGHTUSERDATA = 2, //LUA_TLIGHTUSERDATA
    TNUMBER = 3, //LUA_TNUMBER
    TSTRING = 4, //LUA_TSTRING
    TTABLE = 5, //LUA_TTABLE
    TFUNCTION = 6, //LUA_TFUNCTION
    TUSERDATA = 7, //LUA_TUSERDATA
    TTHREAD = 8, //LUA_TTHREAD
    NUMTYPES = TTHREAD + 2
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
