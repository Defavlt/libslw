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

#ifndef SLW_STATE_H
#define SLW_STATE_H

#include <exception>
#include <vector>

#include "slw/types.h"
#include "slw/stdout.h"

struct lua_State;

namespace slw {

class Call;
class Field;
class State;

class State
{
private:
    static const char *const magic;

    int ref();
    int deref();

public:

    int refs();

    friend class slw::Call;
    friend class slw::Field;

    State();

    State(State &);
    State(lua_State *);

    ~State();

    bool
    load(const char *str, const bool str_is_file = true);

    template<typename _value_t>
    _value_t pop(_value_t _defaults)
    {
        pop(_defaults);
        return _defaults;
    }

    /* \brief Pop a value from the stack
     * \return true if the stack isn't empty and there's an actual
     *     value
     *
     * \param the pop'd value
     * \param force will force a pop if true
     */
    bool pop(slw::string_t &, bool force = false);
    bool pop(number_t &, bool force = false);
    bool pop(long &, bool force = false);
    bool pop(int &, bool force = false);
    bool pop(char &, bool force = false);
    bool pop(bool &, bool force = false);

    bool remove(slw::string_t &, int offset);
    bool remove(slw::number_t &, int offset);
    bool remove(long &, int offset);
    bool remove(int &, int offset);
    bool remove(char &, int offset);
    bool remove(bool &, int offset);

    /* \brief Pop the value at the top of the stack
     */
    bool pop();

    template<typename _value_t>
    _value_t peek(_value_t _defaults)
    {
        peek(_defaults);
        return _defaults;
    }

    /* \brief Peek at the stack - optionally from an offset
     * \return true if the stack isn't empty and there's an actual
     *     value
     *
     * \param v the value in the stack
     **/
    bool peek(slw::string_t &, int offset = -1);
    bool peek(number_t &, int offset = -1);
    bool peek(long &, int offset = -1);
    bool peek(int &, int offset = -1);
    bool peek(char &, int offset = -1);
    bool peek(bool &, int offset = -1);

    /* \brief push a value to the top of the stack
     **/
    void push(slw::string_t);
    void push(number_t);
    void push(long);
    void push(int);
    void push(char);
    void push(bool);

    /* \brief push a nil value to the top of the stack
     **/
    void push();

    void dostring(slw::string_t);
    void dofile(slw::string_t);

    /* \brief the current size of the stack
     **/
    int size();

    int type(int index = -1);
    slw::string_t type_name(int);
    int top();

    lua_State *state;
};
} //namespace slw

#endif // SLW_STATE_H
