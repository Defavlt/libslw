////////////////////////////////////////////////////////////
/// \brief Functional member registration helper
///
/// \details
/// For extra Quality of Life:
///  Given members
///   int C::F0(int);
///   int C::F1();
///
/// * hpp:
///   struct C {
///     slw_register_member(F0, int, int);
///     slw_register_membeR(F1, int);
///   };
///
/// \see slw_register_member_construct
/// \see slw_register_class_construct
/// \see slw_register_class
////////////////////////////////////////////////////////////
#define slw_register_member(Identifier, ...) \
    typedef slw::callable_t<__VA_ARGS__> Identifier##_type_info_t; \
    Identifier##_type_info_t Identifier##_type_info; \
    slw::reference Identifier##_slw_ref

////////////////////////////////////////////////////////////
/// \brief Functional member initialization helper
///
/// \details
/// For extra Quality of Life:
///  Given members
///   int C::F0(int);
///   int C::F1();
///
/// * cpp:
///   using namespace std::placeholders;
///   C::C(slw::shared_state L)
///     slw_register_member_construct(C, F0, this, L,, _1),
///     slw_register_member_construct(C, F1, this, L)
///   {}
///
/// \details Either add an extra , before the variadic arguments if non-zero,
///          or uze slw_register_construct_nz
/// \see slw_register_member_construct_nz
/// \see slw_register_member
/// \see slw_register_class
/// \see slw_register_class_construct
////////////////////////////////////////////////////////////
#define slw_register_member_construct(Scope, Identifier, Owner, State, UNUSED, ...) \
    Identifier##_type_info(State, std::bind(&Scope::Identifier, Owner, ##__VA_ARGS__)), \
    Identifier##_slw_ref(State, Scope##_slw_ref, #Identifier, slw::make_callable(State, Identifier##_type_info))

#define slw_register_member_construct_nz(Scope, Identifier, Owner, State, ...) \
    slw_register_member_construct(Scope, Identifier, Owner, State,, __VA_ARGS__)

////////////////////////////////////////////////////////////
/// \brief Functional table registration helper
/// \details
/// C::C(slw::shared_state L)
///  slw_register_class_construct(L, C, "C")
/// {}
///
/// \see slw_register_class_construct
////////////////////////////////////////////////////////////
#define slw_register_class(Scope) \
    slw::reference GetClassReference() \
    { return Scope##_slw_ref; \
    } \
    slw::reference Scope##_slw_ref

////////////////////////////////////////////////////////////
/// \brief Functional table initialization helper
/// \details
/// C::C(slw::shared_state L)
///  slw_register_class_construct(L, C, "C")
/// {}
///
/// \see slw_register_class
////////////////////////////////////////////////////////////
#define slw_register_class_construct(State, Scope, LuaIdentifier) \
    Scope##_slw_ref(State, LuaIdentifier, true)

#ifndef FUNCTIONAL_HELPERS_HPP
#define FUNCTIONAL_HELPERS_HPP
#endif // FUNCTIONAL_HELPERS_HPP
