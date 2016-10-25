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
#include "../helpers/peek.hpp"

bool slw::State::peek(slw::string_t &value, int offset)
{
    return slw::peek(state, value, offset);
}

bool slw::State::peek(number_t &value, int offset)
{
    return slw::peek(state, value, offset);
}

bool slw::State::peek(long &value, int offset)
{
    return slw::peek(state, value, offset);
}

bool slw::State::peek(int &value, int offset)
{
    return slw::peek(state, value, offset);
}

bool slw::State::peek(char &value, int offset)
{
    return slw::peek(state, value, offset);
}

bool slw::State::peek(bool &value, int offset)
{
    return slw::peek(state, value, offset);
}
