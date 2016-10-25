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
#include "../helpers/pop.hpp"

bool slw::State::pop(slw::string_t &value, bool force)
{
    return slw::pop(state, value, force);
}

bool slw::State::pop(number_t &value, bool force)
{
    return slw::pop(state, value, force);
}

bool slw::State::pop(long &value, bool force)
{
    return slw::pop(state, value, force);
}

bool slw::State::pop(int &value, bool force)
{
    return slw::pop(state, value, force);
}

bool slw::State::pop(char &value, bool force)
{
    return slw::pop(state, value, force);
}

bool slw::State::pop(bool &value, bool force)
{
    return slw::pop(state, value, force);
}

bool slw::State::pop()
{
    return slw::pop(state);
}
