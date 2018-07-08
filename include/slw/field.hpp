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

#ifndef SLW_TABLE_H
#define SLW_TABLE_H

#include "slw/get_field.hpp"
#include "slw/state.hpp"
#include "slw/types.hpp"

namespace slw {
namespace internal {

template<typename _field_t, typename _value_t>
struct setter
{
    typedef _field_t field_t;
    typedef _value_t value_t;

    field_t &field;
    slw::string_t name;
    int index;
    value_t value;

    inline field_t &operator =(value_t _value)
    {
        value = _value;
        field = *this;
        return field;
    }

    inline operator value_t()
    {
        field.get(name, value, true);
        return value;
    }

//private:
//    Setter();
};
}

/* \brief Create or modify a field
 *
 * \usage
 *   State st;
 *   Field field(st);
 *   auto age = field.field<int>("age");
 *   age = 25;
 **/
class Field
{
    friend class slw::internal::setter<slw::Field, slw::string_t>;
    friend class slw::internal::setter<slw::Field, slw::number_t>;
    friend class slw::internal::setter<slw::Field, long>;
    friend class slw::internal::setter<slw::Field, int>;
    friend class slw::internal::setter<slw::Field, char>;
    friend class slw::internal::setter<slw::Field, bool>;

public:

    Field(slw::State &state);

    void table(int narr = 0, int nrec = 0);

    template<typename _value_t, typename _setter_t = slw::internal::setter<Field, _value_t>>
    _setter_t field(slw::string_t _name)
    {
        return _setter_t {*this, _name, index};
    }

    template<typename _value_t, typename _setter_t = slw::internal::setter<Field, _value_t>>
    _setter_t field(slw::string_t _name, _value_t _defaults)
    {
        return _setter_t {*this, _name, index, _defaults};
    }

    template<typename _value_t>
    Field &operator =(slw::internal::setter<Field, _value_t> &rhs)
    {
        set(rhs.name, rhs.index, rhs.value);
        return *this;
    }

    template<typename _value_t>
    inline _value_t get(slw::string_t name, _value_t defaults)
    {
        get(name, defaults);
        return defaults;
    }

    bool get(slw::string_t &name, slw::string_t &, bool force = false);
    bool get(slw::string_t &name, slw::number_t &, bool force = false);
    bool get(slw::string_t &name, long &, bool force = false);
    bool get(slw::string_t &name, int &, bool force = false);
    bool get(slw::string_t &name, char &, bool force = false);
    bool get(slw::string_t &name, bool &, bool force = false);

    operator slw::string_t();
    operator slw::number_t();
    operator long();
    operator int();
    operator char();
    operator bool();

private:
    void set(slw::string_t &name, int index, slw::string_t);
    void set(slw::string_t &name, int index, slw::number_t);
    void set(slw::string_t &name, int index, long);
    void set(slw::string_t &name, int index, int);
    void set(slw::string_t &name, int index, char);
    void set(slw::string_t &name, int index, bool);

    slw::State &state;
    slw::string_t name;

    /* the current index, defaulted to slw::internal::indexes::globals
     * */
    int index;
};

} //namespace slw

#endif//SLW_TABLE_H
