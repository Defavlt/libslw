#include "slw/variant/bool.hpp"
#include "lua.hpp"

////////////////////////////////////////////////////////////
/// bool constructors
////////////////////////////////////////////////////////////
#include "variant/variant_ctors.hpp"
variant_ctors(slw::boolean)

////////////////////////////////////////////////////////////
/// bool equality operators
////////////////////////////////////////////////////////////
#include "variant/variant_op_eq.hpp"
variant_op_eq(slw::boolean, ==)
variant_op_eq(slw::boolean, !=)

////////////////////////////////////////////////////////////
/// bool deref operator
////////////////////////////////////////////////////////////
slw::boolean::expected_value_type slw::boolean::operator *()
{
    M_ref.push();
    // I wish I could use my macro here, but alas...
    // How hard would it be to add a lua_tobooleanx that takes
    // a pointer, but leaves it unused?
    expected_value_type r = lua_toboolean(M_state.get(), -1);
    lua_pop(M_state.get(), 1);
    return r;
}

slw::boolean::expected_value_type slw::boolean::operator *() const
{
    slw::reference &ref = *const_cast<slw::reference *>(&M_ref);
    expected_value_type r = lua_toboolean(M_state.get(), -1);
    lua_pop(M_state.get(), 1);
    return r;
}

////////////////////////////////////////////////////////////
/// bool assign operator
////////////////////////////////////////////////////////////
#include "variant/variant_op_assign.hpp"
variant_op_assign(slw::boolean, lua_pushboolean)
