#ifndef SLW_UTILITY_HPP
#define SLW_UTILITY_HPP

#include "slw/state.hpp"
#include "slw/types.hpp"

#include "slw/reference.hpp"
#include "slw/exception.hpp"

#include <exception>
#include <type_traits>

namespace slw {

template<typename Fp>
struct binder {
    binder(Fp fp) : M_fp(fp) {}
    ~binder() { M_fp(); }

private:
    Fp M_fp;
};

template<typename Fpre, typename Fpost>
////////////////////////////////////////////////////////////
/// \brief Binds a pre-, and post-action to the construction, and destruction
/// of an instance of slw::binder.
/// When the object is sent out of scope, the action \c Fpost is called.
////////////////////////////////////////////////////////////
inline binder<Fpost> bind(Fpre &&pre, Fpost &&post)
{
    binder<Fpre> { pre };
    return binder<Fpost> { post };
}

////////////////////////////////////////////////////////////
/// \brief Check whether a reference is of the specified type
/// \return True if it is of the type
/// \throws slw::no_type_specialization
////////////////////////////////////////////////////////////
template<typename Tp>
bool is(slw::shared_state, slw::reference &)
{ throw slw::no_type_specialization();
}

////////////////////////////////////////////////////////////
/// \brief Check whether a reference is of the specified type
/// \return True if it is of the type
/// \throws slw::no_type_specialization
////////////////////////////////////////////////////////////
template<>
bool is<slw::number_t>(slw::shared_state, slw::reference &);

////////////////////////////////////////////////////////////
/// \brief Check whether a reference is of the specified type
/// \return True if it is of the type
/// \throws slw::no_type_specialization
////////////////////////////////////////////////////////////
template<>
bool is<slw::uint_t>(slw::shared_state, slw::reference &);

////////////////////////////////////////////////////////////
/// \brief Check whether a reference is of the specified type
/// \return True if it is of the type
/// \throws slw::no_type_specialization
////////////////////////////////////////////////////////////
template<>
bool is<slw::int_t>(slw::shared_state, slw::reference &);

////////////////////////////////////////////////////////////
/// \brief Check whether a reference is of the specified type
/// \return True if it is of the type
/// \throws slw::no_type_specialization
////////////////////////////////////////////////////////////
template<>
bool is<slw::string_t>(slw::shared_state, slw::reference &);

////////////////////////////////////////////////////////////
/// \brief Check whether a reference is of the specified type
/// \return True if it is of the type
/// \throws slw::no_type_specialization
////////////////////////////////////////////////////////////
template<>
bool is<slw::table_t>(slw::shared_state, slw::reference &);

/// \brief Check whether a reference is of the specified type
/// \return True if it is of the type
/// \throws slw::no_type_specialization
////////////////////////////////////////////////////////////
template<>
bool is<slw::bool_t>(slw::shared_state, slw::reference &);

////////////////////////////////////////////////////////////
/// \brief Coerce a reference into a type Tp
/// \return The value of the reference coerced into the type Tp
/// \throws slw::no_type_specialization
////////////////////////////////////////////////////////////
template<typename Tp>
Tp as(slw::shared_state, slw::reference &)
{ throw slw::no_type_specialization();
}

////////////////////////////////////////////////////////////
/// \brief Coerce a reference into a type Tp
/// \return The value of the reference coerced into the type Tp
/// \throws slw::no_type_specialization
////////////////////////////////////////////////////////////
template<>
slw::number_t as<slw::number_t>(slw::shared_state, slw::reference &);

////////////////////////////////////////////////////////////
/// \brief Coerce a reference into a type Tp
/// \return The value of the reference coerced into the type Tp
/// \throws slw::no_type_specialization
////////////////////////////////////////////////////////////
template<>
slw::uint_t as<slw::uint_t>(slw::shared_state, slw::reference &);

////////////////////////////////////////////////////////////
/// \brief Coerce a reference into a type Tp
/// \return The value of the reference coerced into the type Tp
/// \throws slw::no_type_specialization
////////////////////////////////////////////////////////////
template<>
slw::int_t as<slw::int_t>(slw::shared_state, slw::reference &);

////////////////////////////////////////////////////////////
/// \brief Coerce a reference into a type Tp
/// \return The value of the reference coerced into the type Tp
/// \throws slw::no_type_specialization
////////////////////////////////////////////////////////////
template<>
slw::string_t as<slw::string_t>(slw::shared_state, slw::reference &);

////////////////////////////////////////////////////////////
/// \brief Coerce a reference into a type Tp
/// \return The value of the reference coerced into the type Tp
/// \throws slw::no_type_specialization
////////////////////////////////////////////////////////////
template<>
slw::table_t as<slw::table_t>(slw::shared_state, slw::reference &);

////////////////////////////////////////////////////////////
/// \brief Coerce a reference into a type Tp
/// \return The value of the reference coerced into the type Tp
/// \throws slw::no_type_specialization
////////////////////////////////////////////////////////////
template<>
slw::bool_t as<slw::bool_t>(slw::shared_state, slw::reference &);

template<typename Ta>
struct assert {
    template<typename Tb>
    bool is(slw::shared_state state, Tb &v)
    { return is(state, v)? true: throw slw::unexpected_type();
    }
};

slw::reference registry(slw::shared_state);
slw::reference globals(slw::shared_state);

} //slw

#endif//SLW_UTILITY_HPP
