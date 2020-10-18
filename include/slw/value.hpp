#ifndef SLW_VALUE_HPP
#define SLW_VALUE_HPP

#include "slw/state.hpp"
#include "slw/reference.hpp"
#include "slw/exception.hpp"
#include "slw/variant/variant.hpp"

namespace slw {

template<typename Tp>
struct value_t
{
    typedef Tp index_type;
};

template<> struct value_t<slw::string_t> {
    typedef slw::int_t index_type;

    ////////////////////////////////////////////////////////////
    /// \brief Construct an opaque value, in the table t, using index k.
    /// \param state The Lua state
    /// \param t The table in which the value will be constructed
    /// \param k The index at which to construct the value
    ////////////////////////////////////////////////////////////
    value_t(slw::shared_state state, const slw::string_t &t, const slw::string_t &k);

    ////////////////////////////////////////////////////////////
    /// \brief Construct an opaque value, in the table t, using index k.
    /// \param state The Lua state
    /// \param t The table in which the value will be constructed
    /// \param k The index at which to construct the value
    ////////////////////////////////////////////////////////////
    value_t(slw::shared_state state, const slw::string_t &t, index_type k);

    ////////////////////////////////////////////////////////////
    /// \brief Construct an opaque value, in the table t, using index k.
    /// \param state The Lua state
    /// \param t The table in which the value will be constructed
    /// \param k The index at which to construct the value
    ////////////////////////////////////////////////////////////
    value_t(slw::shared_state state, index_type t, const slw::string_t &k);

    ////////////////////////////////////////////////////////////
    /// \brief Construct an opaque value, in the table t, using index k.
    /// \param state The Lua state
    /// \param t The table in which the value will be constructed
    /// \param k The index at which to construct the value
    ////////////////////////////////////////////////////////////
    value_t(slw::shared_state state, index_type t, index_type k);

    ////////////////////////////////////////////////////////////
    /// \brief Construct an opaque value, in the table t, using index k.
    /// \param state The Lua state
    /// \param t The table in which the value will be constructed
    /// \param k The index at which to construct the value
    ////////////////////////////////////////////////////////////
    value_t(slw::shared_state state, slw::reference &t, index_type k);

    ////////////////////////////////////////////////////////////
    /// \brief Construct an opaque value, in the table t, using index k.
    /// \param state The Lua state
    /// \param t The table in which the value will be constructed
    /// \param k The index at which to construct the value
    ////////////////////////////////////////////////////////////
    value_t(shared_state state, slw::reference &t, const slw::string_t &k);

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ////////////////////////////////////////////////////////////
    value_t(const value_t &);

    ////////////////////////////////////////////////////////////
    /// \brief Set this value to the referenced value
    /// \return this value
    ////////////////////////////////////////////////////////////
    value_t &operator =(slw::reference ref);

    ////////////////////////////////////////////////////////////
    /// \brief Set this value to the other value
    /// \return this value
    ////////////////////////////////////////////////////////////
    value_t &operator =(value_t);

    ////////////////////////////////////////////////////////////
    /// \brief Assign the value at the top of the stack.
    /// \see slw::reference::assign
    ////////////////////////////////////////////////////////////
    void assign();

    ////////////////////////////////////////////////////////////
    /// \brief Push this value to the top of the stack
    /// \see slw::reference::push
    ////////////////////////////////////////////////////////////
    void push();

private:
    slw::shared_state   M_state;
    slw::reference      M_ref;
    slw::string_t       M_idx;
};

template<> struct value_t<slw::int_t> {
    typedef slw::int_t index_type;

    ////////////////////////////////////////////////////////////
    /// \brief Construct an opaque value, in the table t, using index k.
    /// \param state The Lua state
    /// \param t The table in which the value will be constructed
    /// \param k The index at which to construct the value
    ////////////////////////////////////////////////////////////
    value_t(slw::shared_state state, const slw::string_t &t, const slw::string_t &k);

    ////////////////////////////////////////////////////////////
    /// \brief Construct an opaque value, in the table t, using index k.
    /// \param state The Lua state
    /// \param t The table in which the value will be constructed
    /// \param k The index at which to construct the value
    ////////////////////////////////////////////////////////////
    value_t(slw::shared_state state, const slw::string_t &t, index_type k);

    ////////////////////////////////////////////////////////////
    /// \brief Construct an opaque value, in the table t, using index k.
    /// \param state The Lua state
    /// \param t The table in which the value will be constructed
    /// \param k The index at which to construct the value
    ////////////////////////////////////////////////////////////
    value_t(slw::shared_state state, index_type t, const slw::string_t &k);

    ////////////////////////////////////////////////////////////
    /// \brief Construct an opaque value, in the table t, using index k.
    /// \param state The Lua state
    /// \param t The table in which the value will be constructed
    /// \param k The index at which to construct the value
    ////////////////////////////////////////////////////////////
    value_t(slw::shared_state state, index_type t, index_type k);

    ////////////////////////////////////////////////////////////
    /// \brief Construct an opaque value, in the table t, using index k.
    /// \param state The Lua state
    /// \param t The table in which the value will be constructed
    /// \param k The index at which to construct the value
    ////////////////////////////////////////////////////////////
    value_t(slw::shared_state state, slw::reference &t, index_type k);

    ////////////////////////////////////////////////////////////
    /// \brief Construct an opaque value, in the table t, using index k.
    /// \param state The Lua state
    /// \param t The table in which the value will be constructed
    /// \param k The index at which to construct the value
    ////////////////////////////////////////////////////////////
    value_t(shared_state state, slw::reference &t, const slw::string_t &k);

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ////////////////////////////////////////////////////////////
    value_t(const value_t &);

    ////////////////////////////////////////////////////////////
    /// \brief Set this value to the referenced value
    /// \return this value
    ////////////////////////////////////////////////////////////
    value_t &operator =(slw::reference);

    ////////////////////////////////////////////////////////////
    /// \brief Set this value to the other value
    /// \return this value
    ////////////////////////////////////////////////////////////
    value_t &operator =(value_t);

    ////////////////////////////////////////////////////////////
    /// \brief Assign the value at the top of the stack.
    /// \see slw::reference::assign
    ////////////////////////////////////////////////////////////
    void assign();

    ////////////////////////////////////////////////////////////
    /// \brief Push this value to the top of the stack
    /// \see slw::reference::push
    ////////////////////////////////////////////////////////////
    void push();

private:
    slw::shared_state   M_state;
    slw::reference      M_ref;
    slw::int_t          M_idx;
};

typedef slw::value_t<slw::int_t>    array_value;
typedef slw::value_t<slw::string_t> value;

}/*ns slw*/

#endif/*SLW_VALUE_HPP*/
