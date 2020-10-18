////////////////////////////////////////////////////////////
/// \brief slw::variant standard constructors
/// \param type the type of this class
////////////////////////////////////////////////////////////
#define variant_numeric_op_deref(type, conv)                            \
type::expected_value_type type::operator *() const                      \
{                                                                       \
    slw::reference &ref = *const_cast<slw::reference *>(&M_ref);        \
    ref.push();                                                         \
    type::expected_value_type r = conv(M_state.get(), -1, nullptr);     \
    lua_pop(M_state.get(), 1);                                          \
    return r;                                                           \
}
