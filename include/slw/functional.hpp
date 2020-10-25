#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP

#include "slw/state.hpp"
#include "slw/reference.hpp"
#include "slw/utility.hpp"

#include "slw/range/type_range.hpp"
#include "slw/range/apply.hpp"

#include <functional>
#include <tuple>

namespace slw {

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
/// reference r_call = make_callable(state, &call);
/// // /* assign the callable to a global parameter here...*/
/// luaL_dostring(state.get(), "return a_native_function(/*...*/)");
///
/// // Now, top -1 contains whatever a_native_function returned.
/// //  Function arguments are cleared, from the stack, as well.
///
/// \endcode
////////////////////////////////////////////////////////////
template<typename Rt, typename ...Rp>
struct functional {
    typedef Rt                  return_type;
    typedef std::function<Rt()> functional_type;

    functional(shared_state state, functional_type functor)
        : M_state(state)
        , M_functor(functor)
    {}

    return_type operator ()()
    { return M_functor();
    }

private:
    slw::shared_state M_state;
    functional_type M_functor;
};

template<typename Rt, typename Fp, typename ...Rp>
struct functional<Rt, Fp, Rp...> {
    typedef Rt                                  return_type;
    typedef std::function<Rt(Fp, Rp...)>        functional_type;

    functional(shared_state state, functional_type functor)
        : M_state(state)
        , M_functor(functor)
    {}

    return_type operator ()()
    {
        std::size_t argsc = sizeof...(Rp) + 1;
        slw::size_t size = slw::get_size(M_state);

        if (size != argsc || !size)
            throw slw::unexpected_argument_range(M_state, size, argsc);

        // type validation lambda of supplied parameters
        auto val = [&](auto _idx, auto *_tp) -> bool {
            typedef remove_pointer(_tp) element_type;
            slw::reference idx_ref { M_state, _idx() };
            return slw::is<element_type>(idx_ref);
        };

        // validate the type of all supplied parameters,
        //  requiring all to be valid
        // TODO: Fix inlining for val
        if (!range::type_range_t<Fp, Rp...>::all(val))
            throw slw::unexpected_type(M_state);

        // simple unpacking of supplied arguments into a tuple
        auto cat = [](auto &&...args) {
            return std::tuple_cat(args...);
        };

        // retrieve a value at a specific index coerced into a specific type
        auto collect = [&](auto _idx, auto *_tp) {
            typedef remove_pointer(_tp) element_type;
            slw::reference ref { M_state, _idx() };
            return slw::as<element_type>(ref);
        };

        // now iterate through all parameters and collect their value,
        //  and unpack as actual parameters into the call to M_functor
        return slw::range::apply<sizeof...(Rp) + 1>(
            M_functor,
            range::type_range_t<Fp, Rp...>::collect(cat, collect));
    }

    return_type operator ()(Fp &&arg0, Rp &&...args)
    { return functor(arg0, args...);
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

template<typename R, typename ...P>
struct callable_t : public callable {
    typedef slw::functional<R, P...> functional_type;
    typedef typename functional_type::functional_type function_type;

    explicit callable_t(slw::shared_state state, function_type fn)
        : M_state(state)
        , M_functor(state, fn)
    {
    }

    int operator ()() override
    {
        slw::push(M_state, M_functor());
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

    int operator ()() override
    {
        M_functor();
        return 0;
    }

private:
    slw::shared_state M_state;
    functional_type M_functor;
};

slw::reference make_callable(slw::shared_state state, slw::callable &f);

} /*ns slw*/

#endif // FUNCTIONAL_HPP
