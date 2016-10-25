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

#include "slw/state.h"
#include "../helpers/remove.hpp"

bool slw::State::remove(slw::string_t &value, int offset)
{
    bool is_type = peek(value, offset);
    slw::remove(state, offset);
    return is_type;
}

bool slw::State::remove(slw::number_t &value, int offset)
{
    bool is_type = peek(value, offset);
    slw::remove(state, offset);
    return is_type;
}

bool slw::State::remove(long &value, int offset)
{
    bool is_type = peek(value, offset);
    slw::remove(state, offset);
    return is_type;
}

bool slw::State::remove(int &value, int offset)
{
    bool is_type = peek(value, offset);
    slw::remove(state, offset);
    return is_type;
}

bool slw::State::remove(char &value, int offset)
{
    bool is_type = peek(value, offset);
    slw::remove(state, offset);
    return is_type;
}

bool slw::State::remove(bool &value, int offset)
{
    bool is_type = peek(value, offset);
    slw::remove(state, offset);
    return is_type;
}
