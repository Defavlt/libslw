////////////////////////////////////////////////////////////
/// \brief slw::variant assignment operator generator
/// \param type the type this member belong to
/// \param push the Lua push function used for the underlying type of \c type
////////////////////////////////////////////////////////////
#define variant_op_assign(type, push)                                   \
type &type::operator =(const type::expected_value_type &value)          \
{                                                                       \
    push(M_state.get(), value);                                         \
    M_ref.assign();                                                     \
    return *this;                                                       \
}
