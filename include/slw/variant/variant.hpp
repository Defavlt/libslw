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

#ifndef SLW_VARIANT_VARIANT_HPP
#define SLW_VARIANT_VARIANT_HPP

#include "slw/state.hpp"
#include "slw/reference.hpp"
#include "slw/value.hpp"
#include "slw/types.hpp"
#include "slw/exception.hpp"

namespace slw {

template<typename Tp>
////////////////////////////////////////////////////////////
/// \brief The variant struct
/// \param Tp the expected type
/// \see slw::number_t, slw::uint_t, slw::int_t, slw::string_t
////////////////////////////////////////////////////////////
struct variant {
    typedef Tp expected_value_type;
    static constexpr type_e internal_value_type = TNONE;

    ////////////////////////////////////////////////////////////
    /// \brief Construct a variant, using the specified state
    /// \param state The Lua state
    /// \throws slw::
    ////////////////////////////////////////////////////////////
    variant(slw::shared_state state)
        : M_state(state)
        , M_ref(state)
    { throw no_type_specialization();
    }

    ////////////////////////////////////////////////////////////
    /// \brief Construct a variant, using the specified state,
    /// and referencing the specified object.
    /// \param state The Lua state
    /// \param name The path to the object, e.g. `person.skills.social`
    /// for a persons social skills.
    /// \throws slw::no_type_specialization
    variant(slw::shared_state state, const slw::string_t &name)
        : M_state(state)
        , M_ref(state, name)
    { throw no_type_specialization();
    }

    ////////////////////////////////////////////////////////////
    /// \brief Construct a variant, using the specified state,
    /// and dereferencing the specified reference.
    /// \param state The Lua state
    /// \param ref The reference
    /// \throws slw::no_type_specialization
    ////////////////////////////////////////////////////////////
    variant(slw::shared_state state, const slw::reference &ref)
        : M_state(state)
        , M_ref(ref)
    { throw no_type_specialization();
    }

    ////////////////////////////////////////////////////////////
    /// \brief Construct a variant, using the specified state,
    /// and pointing to the specified index.
    /// \param state The Lua State
    /// \param idx The index
    ////////////////////////////////////////////////////////////
    variant(slw::shared_state state, slw::int_t idx)
        : M_state(state)
        , M_ref(idx)
    { throw no_type_specialization();
    }

    ////////////////////////////////////////////////////////////
    /// \brief Construct a variant pointing to the specified referenced value.
    /// \param ref
    ////////////////////////////////////////////////////////////
    variant(const slw::reference &ref)
        : M_state(ref.get_state())
        , M_ref(ref)
    { throw no_type_specialization();
    }

    ////////////////////////////////////////////////////////////
    /// \brief Dereference operator for Tp, or the expected_value_type
    /// \throws slw::no_type_specialization
    ////////////////////////////////////////////////////////////
    expected_value_type operator *()
    { throw no_type_specialization();
    }

    ////////////////////////////////////////////////////////////
    /// \brief Assignment operator for Tp, or the expected_value_type.
    /// \return This variant
    /// \throws slw::sno_type_specialization
    ////////////////////////////////////////////////////////////
    variant<Tp> &operator =(const expected_value_type &)
    { throw no_type_specialization();
    }

    ////////////////////////////////////////////////////////////
    /// \brief operator = Assignment operator from reference
    /// \return This variant
    /// \throws slw::sno_type_specialization
    ////////////////////////////////////////////////////////////
    variant<Tp> operator =(const reference &)
    { throw no_type_specialization();
    }

    ////////////////////////////////////////////////////////////
    /// \brief Equality operator  for Tp, or the expected_value_type
    /// \return Whether the object this variant references is
    /// equal to the specified value
    /// \throws slw::no_type_specialization
    ////////////////////////////////////////////////////////////
    bool operator ==(const expected_value_type &)
    { throw no_type_specialization();
    }

    ////////////////////////////////////////////////////////////
    /// \brief Inequality operator  for Tp, or the expected_value_type
    /// \return Whether the object this variant references is
    /// not equal to the specified value
    /// \throws slw::no_type_specialization
    ////////////////////////////////////////////////////////////
    bool operator !=(const expected_value_type &)
    { throw no_type_specialization();
    }

    ////////////////////////////////////////////////////////////
    /// \brief Equality operator another variant<Tp>.
    /// \return Whether the object this variant references is
    /// equal to the value of the other specified reference.
    /// \throws slw::no_type_specialization
    ////////////////////////////////////////////////////////////
    bool operator ==(variant<expected_value_type> &)
    { throw no_type_specialization();
    }

    ////////////////////////////////////////////////////////////
    /// \brief Inequality operator another variant<Tp>.
    /// \return Whether the object this variant references is
    /// not equal to the value of the other specified reference.
    /// \throws slw::no_type_specialization
    ////////////////////////////////////////////////////////////
    bool operator !=(variant<expected_value_type> &)
    { throw no_type_specialization();
    }

    ////////////////////////////////////////////////////////////
    /// \brief Get the reference this variant holds
    /// \param The key
    /// \return The reference
    /// \throws slw::no_type_specialization
    ////////////////////////////////////////////////////////////
    template<typename Tpp> Tpp get(const slw::string_t &)
    { throw no_type_specialization();
    }

    ////////////////////////////////////////////////////////////
    /// \brief Get the reference this variant holds
    /// \param The key
    /// \return The reference
    /// \throws slw::no_type_specialization
    ////////////////////////////////////////////////////////////
    template<typename Tpp> Tpp get(slw::int_t)
    { throw no_type_specialization();
    }

    template<typename Tpp> bool is()
    // No type specialization, but we're simply returning false.
    { return false;
    }

    ////////////////////////////////////////////////////////////
    /// \brief Get the reference this variant holds
    /// \param The key
    /// \return The reference
    ////////////////////////////////////////////////////////////
    slw::reference get()
    { return M_ref;
    }

private:

    ////////////////////////////////////////////////////////////
    /// \brief The Lua state
    ////////////////////////////////////////////////////////////
    slw::shared_state M_state;

    ////////////////////////////////////////////////////////////
    /// \brief The object reference
    ////////////////////////////////////////////////////////////
    slw::reference M_ref;
};

template<>
struct variant<slw::function_t> {
    // TODO possibly implement calling from var?
};

template<typename Tp>
bool same_type(slw::reference &r) {
    return slw::variant<Tp>::internal_value_type == r.type();
}

template<type_e Te>
bool same_type(slw::reference &r) {
    return Te == r.type();
}

} //namespace slw

////////////////////////////////////////////////////////////
/// \class slw::variant
///
/// slw::variant is a managed reference to a value in the lua state.
///
/// Usage example:
/// \code
/// // Declare, and create a Lua state
/// slw::shared_state state = slw::make_state();
/// slw::variant v = slw::number(state, "some_object");
/// // Assign to the object
/// v = 52.0;
/// // Retrieve the object value
/// std::cout << std::to_string(*v) << std::endl;
/// \endcode
////////////////////////////////////////////////////////////

#endif // SLW_VARIANT_VARIANT_HPP
