#ifndef SLW_REFERENCE_HPP
#define SLW_REFERENCE_HPP

#include "state.hpp"
#include "types.hpp"

namespace slw {

////////////////////////////////////////////////////////////
/// \brief Create a reference to an object
/// \example
///   slw::shared_state state = slw::make_state();
///   slw::reference age ("age");
///   slw::type_e type = age.type();
///   bool is_number = slw::is<slw::number_t>(age);
///   slw::int_t i = slw::as<slw::int_t>(age);
///
/// \see luaL_ref
/// \see slw::state, slw::is, slw::as, slw::push
////////////////////////////////////////////////////////////
struct reference {

    ////////////////////////////////////////////////////////////
    /// \brief Frees this reference.
    ///
    /// If it is the last reference to the object,
    /// the object will be collected next the Lua garbage collector
    /// is run.
    ////////////////////////////////////////////////////////////
    ~reference();

    ////////////////////////////////////////////////////////////
    /// \brief Construct a reference to the object at the top of the stack,
    /// or, if the stack is empty, a null reference.
    ////////////////////////////////////////////////////////////
    reference(slw::shared_state state);

    ////////////////////////////////////////////////////////////
    /// \brief Construct a reference to the object at the given, valid, index.
    /// \param state The Lua state
    /// \param idx The valid index
    ////////////////////////////////////////////////////////////
    reference(slw::shared_state state, slw::int_t idx);

    ////////////////////////////////////////////////////////////
    /// \brief Construct a reference to the named object
    ////////////////////////////////////////////////////////////
    reference(slw::shared_state state, const slw::string_t &name);

    ////////////////////////////////////////////////////////////
    /// \brief Construct a reference to the object, *in* the object
    ///        at the given, valid index. E.g. get a reference to idx[name].
    /// \param idx The valid index
    /// \param name The name of the object
    ////////////////////////////////////////////////////////////
    reference(slw::shared_state state, slw::int_t idx, const slw::string_t &name);

    ////////////////////////////////////////////////////////////
    /// \brief Construct a reference to t[k]
    /// \param t the valid containing object
    /// \param k the key
    ////////////////////////////////////////////////////////////
    reference(slw::shared_state, slw::reference &t, const slw::string_t &k);

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ////////////////////////////////////////////////////////////
    reference(const reference &);

    ////////////////////////////////////////////////////////////
    /// \brief Copy, and assign another reference to this reference,
    /// releasing any already held references
    ////////////////////////////////////////////////////////////
    reference &operator =(reference);

    ////////////////////////////////////////////////////////////
    /// \brief Push to the stack, the object this reference references.
    ////////////////////////////////////////////////////////////
    void push();

    ////////////////////////////////////////////////////////////
    /// \brief Assign the value at the top of the stack, to the object
    ///        this reference, references.
    ///
    /// In other words, the sequence
    /// \code
    /// slw::reference::push();
    /// slw::reference::assign();
    /// \endcode
    ///
    /// is, basically, equivalent to `r = r`
    ////////////////////////////////////////////////////////////
    void assign();

    ////////////////////////////////////////////////////////////
    /// \brief set this[k] = v
    /// \param k the key to assign
    /// \param v the value
    /// \return a reference to the value assigned
    ////////////////////////////////////////////////////////////
    void  assign(const slw::string_t &&, slw::number_t v);

    ////////////////////////////////////////////////////////////
    /// \see slw::reference::push
    ////////////////////////////////////////////////////////////
    void assign(const slw::string_t &&k, slw::uint_t v);

    ////////////////////////////////////////////////////////////
    /// \see slw::reference::push
    ////////////////////////////////////////////////////////////
    void assign(const slw::string_t &&k, slw::int_t v);

    ////////////////////////////////////////////////////////////
    /// \see slw::reference::push
    ////////////////////////////////////////////////////////////
    void assign(const slw::string_t &&k, slw::string_t v);

    ////////////////////////////////////////////////////////////
    /// \see slw::reference::push
    ////////////////////////////////////////////////////////////
    void assign(const slw::string_t &&k, slw::bool_t v);

    ////////////////////////////////////////////////////////////
    /// \see slw::reference::push
    ////////////////////////////////////////////////////////////
    void assign(const slw::string_t &&k, slw::reference &v);

    ////////////////////////////////////////////////////////////
    /// \see slw::reference::push
    ////////////////////////////////////////////////////////////
    void assign(const slw::string_t &&k, const slw::string_t &&value);

    reference operator [](const char *str)
    { return operator [](slw::string_t { str });
    }

    ////////////////////////////////////////////////////////////
    /// \brief Array subscript for accessing fields on the object
    /// this reference references.
    /// \return A reference to the field.
    ////////////////////////////////////////////////////////////
    reference operator [](const slw::string_t &);

    ////////////////////////////////////////////////////////////
    /// \brief Array subscript for accessing array fields on the object
    /// this reference references.
    /// \return A reference to the field.
    ////////////////////////////////////////////////////////////
    reference operator [](const slw::int_t &);

    reference &operator =(reference &ref);

    ////////////////////////////////////////////////////////////
    /// \brief Is this a valid not-nil reference?
    /// \return true if this is a valid reference
    ////////////////////////////////////////////////////////////
    bool valid();

    ////////////////////////////////////////////////////////////
    /// \brief Compare to another reference
    /// \return true if they're the same
    /// \param rhs the other reference
    ////////////////////////////////////////////////////////////
    bool operator ==(const slw::reference &rhs) const;
    bool operator !=(const slw::reference &rhs) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the underlying state
    /// \return slw::shared_state &
    ////////////////////////////////////////////////////////////
    slw::shared_state &get_state();
    const slw::shared_state &get_state() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the type of the value referenced by this reference
    /// \return slw::type_e the type of the value
    ////////////////////////////////////////////////////////////
    slw::type_e type();

    ////////////////////////////////////////////////////////////
    /// \brief Get the type of the value referenced by this reference
    /// \return slw::type_e the type of the value
    ////////////////////////////////////////////////////////////
    template<type_e Te>
    bool is() {
        return Te == type();
    }

private:
    ////////////////////////////////////////////////////////////
    /// \brief The lua_State
    ////////////////////////////////////////////////////////////
    shared_state M_state;

    ////////////////////////////////////////////////////////////
    /// \brief The reference
    ////////////////////////////////////////////////////////////
    slw::int_t M_ref;
};

////////////////////////////////////////////////////////////
/// \brief Make a reference to an object on the stack
/// \param state The lua_State
/// \param name The name of the field, e.g. person.age, or just age
////////////////////////////////////////////////////////////
reference make_reference(slw::shared_state state, const slw::string_t &name);

template<typename Tp> void push(slw::shared_state &state, Tp);

template<> void push<slw::number_t>(slw::shared_state &, slw::number_t);
template<> void push<slw::uint_t>(slw::shared_state &, slw::uint_t);
template<> void push<slw::int_t>(slw::shared_state &, slw::int_t);
template<> void push<slw::bool_t>(slw::shared_state &, slw::bool_t);

void push(slw::shared_state &, const slw::string_t &);
void push(slw::shared_state &, slw::reference &);

////////////////////////////////////////////////////////////
/// \brief push Push a new (empty) table onto the stack.
/// \param state The lua_State
/// \note Really just for convenience
/// \see lua_createtable
////////////////////////////////////////////////////////////
void push(slw::shared_state &state);

}/*ns slw*/

////////////////////////////////////////////////////////////
/// \class slw::reference
///
/// slw::reference is a reference to an object in the Lua state.
/// Upon destruction it automatically dereferences the object in question.
///
/// Usage example:
/// \code
/// // Declare, and create a Lua state
/// slw::shared_state state = slw::make_state();
/// // Allocate a space for some data, eg.
/// slw::push(state);
/// slw::reference our_table { state };
/// // Let's make it circular!
/// // e.g. our_table.our_table == our_table
/// our_table.assign("our_table", our_table);
/// // At this point, scripts Lua-side can't actually access `our_table`.
/// // Let's change that!
/// slw::globals(state).assign("our_table", our_table);
/// \endcode
///
/// \see slw::shared_state, slw::make_state, slw::globals, slw::push

#endif // SLW_REFERENCE_HPP
