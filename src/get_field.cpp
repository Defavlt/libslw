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

#include <cstdlib>
#include <cstring>

#include "slw/get_field.hpp"
#include "slw/types.hpp"
#include "slw/stdout.hpp"

bool slw::get_field(slw::State &state, slw::string_t field)
{
    return slw::get_field(state.state, field);
}

bool slw::get_field(lua_State *state, slw::string_t field)
{
    const unsigned int length = field.size();
    const char *fn_or_obj = field.c_str();

    // the current character index
    unsigned int current_i = 0;

    // the boundary at which each the current level
    // of recursion resides at
    //  i.e. recursion = 6 if table0.
    unsigned int boundary = 0;

    // the level of recursion
    //  i.e. level = 3 if table0.table1.table2
    unsigned int level = 0;

    // the previous character
    char previous_c = fn_or_obj[current_i];

    for ( ; current_i < length; ) {
        const char &current_c = fn_or_obj[current_i];

        // make sure to invalidate multiple '.'
        if ('.' == previous_c) {
            log_error("Unexpected character (%d)\n", current_i);
        }

        switch(current_c) {
        case '.': {
            const unsigned int block_l = current_i - boundary;
            slw::string_t substr = field.substr(boundary, block_l);
            const char *csubstr = substr.c_str();

            if ( !boundary )
                lua_getglobal(state, csubstr);

            else {
                lua_pushstring(state, csubstr);
                ++level;

                if ( !lua_istable(state, -2) ) {
                    log_error("Attempt to index a nil value ('%s')\n", fn_or_obj);

                    lua_pop(state, (int) level);
                    return false;
                }

                else {
                    lua_gettable(state, -2);
                    lua_remove(state, -2);
                }
            }

            ++current_i;
            boundary = current_i;

            break;
        }

        default:
            ++current_i;
            break;
        }

        previous_c = current_c;
    }

    if (!boundary)
        lua_getglobal(state, fn_or_obj);

    else {
        if ( lua_isnil(state, -1)) {
            log_error("Attempt to index a nil value ('%s')\n", fn_or_obj);
            lua_pop(state, (int) level);

            return false;
        }

        else {
            const unsigned int block_l = current_i - boundary;
            slw::string_t substr = field.substr(boundary, block_l);
            const char *csubstr = substr.c_str();

            lua_pushstring(state, csubstr);
            ++level;

            if (lua_istable(state, -2)) {
                lua_gettable(state, -2);
                lua_remove(state, -2);
            }

            else {
                log_error("Attempt to index a nil value ('%s')\n", fn_or_obj);
                lua_pop(state, (int) level);

                return false;
            }
        }
    }

    return true;
}
