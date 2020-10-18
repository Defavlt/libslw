#ifndef SLW_REFERENCE_HPP
#define SLW_REFERENCE_HPP

#include "state.hpp"
#include "types.hpp"

namespace slw {

////////////////////////////////////////////////////////////
/// \brief Create a reference to an object
///
///   State st;
///   Reference age ("age");
///   slw::type_e type = age.type();
///   slw::int_t i = age.as<int>();
///   bool is_number = age.is<slw::number_t>();
///
/// \see luaL_ref
/// \see slw::Field
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
    /// \brief Construct a null reference
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

    reference &operator =(reference &ref)
    {
        ref.push();
        assign();
        return *this;
    }

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
/// // Allocate some data, e.g.
/// // lua_dostring(state.get(), "jack = {}");
/// // Make a reference to "jack"
/// slw::reference c_jack = slw::make_reference(state, "jack");
/// // Clear the global jack slot, e.g.
/// //  lua_dostring(state.get(), "jack = nil");
/// // Now "jack" will live on, at least until "c_jack" go out of scope
/// \endcode
///
/// \see slw::shared_state, slw::make_state

#endif // SLW_REFERENCE_HPP
