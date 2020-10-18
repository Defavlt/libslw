#ifndef SLW_UTILITY_HPP
#define SLW_UTILITY_HPP

#include "slw/state.hpp"
#include "slw/types.hpp"

#include "slw/reference.hpp"
#include "slw/variant/factory.hpp"

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
bool is(slw::shared_state, slw::number &);

////////////////////////////////////////////////////////////
/// \brief Check whether a reference is of the specified type
/// \return True if it is of the type
/// \throws slw::no_type_specialization
////////////////////////////////////////////////////////////
bool is(slw::shared_state, slw::uinteger &);

////////////////////////////////////////////////////////////
/// \brief Check whether a reference is of the specified type
/// \return True if it is of the type
/// \throws slw::no_type_specialization
////////////////////////////////////////////////////////////
bool is(slw::shared_state, slw::integer &);

////////////////////////////////////////////////////////////
/// \brief Check whether a reference is of the specified type
/// \return True if it is of the type
/// \throws slw::no_type_specialization
////////////////////////////////////////////////////////////
bool is(slw::shared_state, slw::string &);

////////////////////////////////////////////////////////////
/// \brief Check whether a reference is of the specified type
/// \return True if it is of the type
/// \throws slw::no_type_specialization
////////////////////////////////////////////////////////////
bool is(slw::shared_state, slw::table &);

/// \brief Check whether a reference is of the specified type
/// \return True if it is of the type
/// \throws slw::no_type_specialization
////////////////////////////////////////////////////////////
bool is(slw::shared_state, slw::boolean &);

template<typename Tp>
////////////////////////////////////////////////////////////
/// \brief Convienience function, \see slw::is
////////////////////////////////////////////////////////////
bool is(slw::shared_state state, slw::reference &ref)
{
    slw::variant<Tp> var { state, ref };
    return is(state, var);
}

template<typename Tp>
////////////////////////////////////////////////////////////
/// \brief Convienience function, \see slw::is
////////////////////////////////////////////////////////////
bool is(slw::shared_state state, slw::int_t idx)
{
    slw::reference ref { state, idx };
    return is<Tp>(state, ref);
}

template<typename Tp>
////////////////////////////////////////////////////////////
/// \brief Convienience function, \see slw::is
////////////////////////////////////////////////////////////
bool is(slw::shared_state state)
{
    return is<Tp>(state, -1);
}

template<typename Ta>
struct assert {
    template<typename Tb>
    bool is(slw::shared_state state, Tb &v)
    { return is(state, v)? true: throw slw::unexpected_type();
    }
};

slw::table registry(slw::shared_state);
slw::table globals(slw::shared_state);

} //slw

#endif//SLW_UTILITY_HPP
