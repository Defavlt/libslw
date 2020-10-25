#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP

#include "lua.hpp"

#include "slw/state.hpp"
#include "slw/reference.hpp"
#include "slw/utility.hpp"

#include "slw/range/type_range.hpp"
#include "slw/range/apply.hpp"

#include <functional>
#include <tuple>

namespace slw {

template<typename Rt, typename ...Ap>
////////////////////////////////////////////////////////////
/// \brief functional is a functor, with built-in argument size, and
/// approximate type validation, as well as return type validation.
///
/// Usage example (from native side):
/// \code
/// typedef slw::callable_t</*...*/> call_type;
///
/// call_type call { state, [](slw::string_t name) -> slw::string_t {
///     return "Hello, " + name + "!";
/// }};
///
/// make_callable(state, slw::globals(state), "a_native_function", &call);
/// luaL_dostring(state.get(), "return a_native_function(/*...*/)");
///
/// // Now, top -1 contains whatever a_native_function returned.
/// //  Function arguments are cleared, from the stack, as well.
///
/// \endcode
////////////////////////////////////////////////////////////
struct functional {
    typedef Rt                                  return_type;
    typedef std::function<Rt(Ap...)>            functional_type;

    functional(shared_state state, functional_type functor)
        : M_state(state)
        , M_functor(functor)
    {}

    return_type operator ()()
    {
        std::size_t argsc = sizeof...(Ap);
        slw::size_t size = slw::get_size(M_state);

        if (size != argsc || !size)
            throw slw::unexpected_argument_range(M_state, size, argsc);

        auto val = [&](auto _idx, auto *_tp) -> bool {
            typedef remove_pointer(_tp) element_type;
            slw::reference idx_ref { M_state, _idx() };
            return slw::is<element_type>(M_state, idx_ref);
        };

        // TODO: Fix inlining for val
        if (!range::type_range_t<Ap...>::all(val))
            throw slw::unexpected_type(M_state);

        auto cat = [](auto &&...args) {
            return std::tuple_cat(args...);
        };

        auto collect = [&](auto _idx, auto *_tp) {
            typedef remove_pointer(_tp) element_type;
            element_type e;
            slw::reference ref { M_state, _idx() };
            return slw::as<element_type>(M_state, ref);
        };

        return slw::range::apply<sizeof...(Ap)>(
            M_functor,
            range::type_range_t<Ap...>::collect(cat, collect));
    }

    return_type operator ()(Ap &&...args)
    { return functor(args...);
    }

private:
    slw::shared_state M_state;
    functional_type M_functor;
};

struct callable {
    virtual int operator ()()
    { return 0;
    }
};

template<typename ...A>
struct callable_t : public callable {
    typedef slw::functional<A...> functional_type;
    typedef typename functional_type::functional_type function_type;

    explicit callable_t(slw::shared_state state, function_type fn)
        : M_state(state)
        , M_functor(state, fn)
    {
    }

    virtual int operator ()() override
    {
        auto return_value = M_functor();

        slw::clear(M_state);
        slw::push(M_state, return_value);
        return 1;
    }

private:
    slw::shared_state M_state;
    functional_type M_functor;
};

template<typename ...A>
struct callable_t<void, A...> : public callable {
    typedef slw::functional<A...> functional_type;
    typedef typename functional_type::functional_type function_type;

    explicit callable_t(slw::shared_state state, function_type fn)
        : M_state(state)
        , M_functor(state, fn)
    {
    }

    virtual int operator ()() override
    {
        M_functor();
        slw::clear(M_state);
        return 0;
    }

private:
    slw::shared_state M_state;
    functional_type M_functor;
};

slw::reference make_callable(slw::shared_state state, slw::callable &f);

} /*ns slw*/

#endif // FUNCTIONAL_HPP
