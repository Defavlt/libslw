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

#include "slw/field.hpp"
#include "slw/state.hpp"

#include "slw/get_field.hpp"

#include "helpers/push.hpp"
#include "helpers/pop.hpp"
#include "lua.hpp"

slw::Field::Field(slw::State &state)
    : state(state)
    , index(slw::internal::indexes::globals)
{
}

void slw::Field::table(int narr, int nrec)
{
    lua_createtable(state.state, narr, nrec);
    index = -1;
}

bool slw::Field::get(slw::string_t &name, slw::string_t &value, bool force)
{
    return
        slw::get_field(state.state, name)
        && state.pop(value, force);
}

bool slw::Field::get(slw::string_t &name, slw::number_t &value, bool force)
{
    return
        slw::get_field(state.state, name)
        && state.pop(value, force);
}

bool slw::Field::get(slw::string_t &name, long &value, bool force)
{
    return
        slw::get_field(state.state, name)
        && state.pop(value, force);
}

bool slw::Field::get(slw::string_t &name, int &value, bool force)
{
    return
        slw::get_field(state.state, name)
        && state.pop(value, force);
}

bool slw::Field::get(slw::string_t &name, char &value, bool force)
{
    return
        slw::get_field(state.state, name)
        && state.pop(value, force);
}

bool slw::Field::get(slw::string_t &name, bool &value, bool force)
{
    return
        slw::get_field(state.state, name)
        && state.pop(value, force);
}

slw::Field::operator slw::string_t()
{
    slw::string_t value;
    get(name, value);
    return value;
}

slw::Field::operator slw::number_t()
{
    slw::number_t value;
    state.pop(value);
    return value;
}

slw::Field::operator long()
{
    long value;
    get(name, value);
    return value;
}

slw::Field::operator int()
{
    int value;
    get(name, value);
    return value;
}

slw::Field::operator char()
{
    char value;
    get(name, value);
    return value;
}

slw::Field::operator bool()
{
    bool value;
    get(name, value);
    return value;
}

void slw::Field::set(slw::string_t &name, int index, slw::string_t value)
{
    slw::push(state.state, value);
    lua_setfield(state.state, index, name.c_str());
}

void slw::Field::set(slw::string_t &name, int index, slw::number_t value)
{
    slw::push(state.state, value);
    lua_setfield(state.state, index, name.c_str());
}

void slw::Field::set(slw::string_t &name, int index, long value)
{
    slw::push(state.state, value);
    lua_setfield(state.state, index, name.c_str());
}

void slw::Field::set(slw::string_t &name, int index, int value)
{
    slw::push(state.state, value);
    lua_setfield(state.state, index, name.c_str());
}

void slw::Field::set(slw::string_t &name, int index, char value)
{
    slw::push(state.state, value);
    lua_setfield(state.state, index, name.c_str());
}

void slw::Field::set(slw::string_t &name, int index, bool value)
{
    slw::push(state.state, value);
    lua_setfield(state.state, index, name.c_str());
}
