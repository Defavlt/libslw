////////////////////////////////////////////////////////////
/// \brief slw::variant equality operator generator
/// \param type the type this member belong to
/// \param eq the equality operator (eg. ==, !=, etc.)
////////////////////////////////////////////////////////////
#define variant_op_eq(type, eq)                                         \
bool type::operator eq(const type::expected_value_type &&value) const   \
{                                                                       \
    return *(*this) eq value;                                           \
}                                                                       \
                                                                        \
bool type::operator eq(const type::expected_value_type &value) const    \
{                                                                       \
    return *(*this) eq value;                                           \
}                                                                       \
                                                                        \
bool type::operator eq(const type &&value) const                        \
{                                                                       \
    return this->M_ref eq value.M_ref;                                  \
}                                                                       \
                                                                        \
bool type::operator eq(const type &value) const                         \
{                                                                       \
    return this->M_ref eq value.M_ref;                                  \
}
