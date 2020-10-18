////////////////////////////////////////////////////////////
/// \brief slw::variant standard constructors
/// \param type the type of this class
////////////////////////////////////////////////////////////
#define variant_ctors(type)                                         \
type::variant(slw::shared_state state)                              \
    : M_state(state)                                                \
    , M_ref(state)                                                  \
{}                                                                  \
                                                                    \
type::variant(slw::shared_state state, const slw::string_t &name)   \
    : M_state(state)                                                \
    , M_ref(state, name)                                            \
{}                                                                  \
                                                                    \
type::variant(slw::shared_state state, const slw::reference &ref)   \
   : M_state(state)                                                 \
   , M_ref(ref)                                                     \
{}                                                                  \
                                                                    \
type::variant(slw::shared_state state, slw::int_t idx)              \
    : M_state(state)                                                \
    , M_ref(state, idx)                                             \
{}                                                                  \
                                                                    \
type::variant(const slw::reference &ref)                            \
    : M_state(ref.get_state())                                      \
    , M_ref(ref)                                                    \
{                                                                   \
}
