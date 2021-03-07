#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP

#include "slw/state.hpp"
#include "slw/reference.hpp"
#include "slw/utility.hpp"

#include "slw/range/type_range.hpp"
#include "slw/range/apply.hpp"

#include <type_traits>
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
/// call_type call { state, std::bind(...)};
///
/// reference r_call = make_callable(state, &call);
/// // /* assign the callable to a global parameter here...*/
/// luaL_dostring(state.get(), "return a_native_function(/*...*/)");
///
/// // Now, top -1 contains whatever a_native_function returned.
/// //  Function arguments are cleared from the stack, as well.
///
/// \endcode
////////////////////////////////////////////////////////////
template<typename Rt, typename ...Rp>
struct functional {
    typedef Rt                  return_type;
    typedef std::function<Rt()> functional_type;

    explicit functional(shared_state state, functional_type functor)
        : M_state(state)
        , M_functor(functor)
    {}

    return_type operator ()()
    {
        std::size_t argsc = sizeof...(Rp);
        slw::size_t size = slw::get_size(M_state);

        if (size)
            throw slw::unexpected_argument_range(M_state, size, argsc);

        return M_functor();
    }

private:
    slw::shared_state M_state;
    functional_type M_functor;
};

template<typename Tr, typename, typename>
struct branch_on_same {
    template<typename Fa, typename Fb>
    static inline Tr resolve(Fa a, Fb b)
    { return b();
    }
};

template<typename Tr, typename Tp>
struct branch_on_same<Tr, Tp, Tp> {
    template<typename Fa, typename Fb>
    static inline Tp resolve(Fa a, Fb b)
    { return a();
    }
};

template<typename Tr, typename, typename>
struct branch_on_same_noreturn {
    template<typename Fa, typename Fb>
    static inline void resolve(Fa a, Fb b)
    { b();
    }
};

template<typename Tr, typename Tp>
struct branch_on_same_noreturn<Tr, Tp, Tp> {
    template<typename Fa, typename Fb>
    static inline void resolve(Fa a, Fb b)
    { a();
    }
};

template<typename Rt, typename Fp, typename ...Rp>
struct functional<Rt, Fp, Rp...> {
    typedef Rt                                  return_type;
    typedef std::function<Rt(Fp, Rp...)>        functional_type;

    explicit functional(shared_state state, functional_type functor)
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

            bool is_same_and_table_t = (std::is_same<slw::reference, element_type>() && slw::is<slw::table_t>(idx_ref));
            bool is_same_and_function_t = (std::is_same<slw::reference, element_type>() && slw::is<slw::function_t>(idx_ref));

            // yes, yes, they're compile-time, and will be optimized by the compiler
            return (std::is_same<slw::reference, element_type>() && slw::is<slw::table_t>(idx_ref))
                    || (std::is_same<slw::reference, element_type>() && slw::is<slw::function_t>(idx_ref))
                    // make sure gcc actually re-compiles when building -.-
                    || (!std::is_same<slw::reference, element_type>() && slw::is<element_type>(idx_ref));
        };

        // validate the type of all supplied parameters,
        //  requiring all to be valid
        // TODO: Fix inlining for val
        if (!range::type_range_t<Fp, Rp...>::all(val))
            throw slw::unexpected_type(M_state);

        // simple unpacking of supplied arguments into a tuple
        auto cat = [](auto ...args) {
            return std::tuple_cat(args...);
        };

        // retrieve a value at a specific index coerced into a specific type
        auto collect = [&](auto _idx, auto *_tp) {
            typedef remove_pointer(_tp) element_type;
            slw::reference ref { M_state, _idx() };
            return branch_on_same<element_type, slw::reference, element_type>::resolve(
                [&]() { return std::move(ref); },
                [&]() { return slw::as<element_type>(ref); }
            );
        };

        // iterate through all parameters and collect their values
        //  type is a tuple of all the different values
        auto arguments = range::type_range_t<Fp, Rp...>::collect(cat, collect);

        // make sure our stack is clear
        slw::clear(M_state);

        // unpack all arguments as actual parameters into the call to M_functor
        return slw::range::apply<sizeof...(Rp) + 1>(
            M_functor,
            arguments
        );
    }

    return_type operator ()(Fp arg0, Rp ...args)
    { return functor(arg0, args...);
    }

private:
    slw::shared_state M_state;
    functional_type M_functor;
};

template<typename Fp, typename ...Rp>
struct functional<void, Fp, Rp...> {
    typedef std::function<void(Fp, Rp...)>        functional_type;

    explicit functional(shared_state state, functional_type functor)
        : M_state(state)
        , M_functor(functor)
    {}

    void operator ()()
    {
        std::size_t argsc = sizeof...(Rp) + 1;
        slw::size_t size = slw::get_size(M_state);

        if (size != argsc || !size)
            throw slw::unexpected_argument_range(M_state, size, argsc);

        // type validation lambda of supplied parameters
        auto val = [&](auto _idx, auto *_tp) -> bool {
            typedef remove_pointer(_tp) element_type;
            slw::reference idx_ref { M_state, _idx() };

            // yes, yes, they're compile-time, and will be optimized by the compiler
            return (std::is_same<slw::reference, element_type>() && slw::is<slw::table_t>(idx_ref))
                    || (std::is_same<slw::reference, element_type>() && slw::is<slw::function_t>(idx_ref))
                    || (!std::is_same<slw::reference, element_type>() && slw::is<element_type>(idx_ref));
        };

        // validate the type of all supplied parameters,
        //  requiring all to be valid
        // TODO: Fix inlining for val
        if (!range::type_range_t<Fp, Rp...>::all(val))
            throw slw::unexpected_type(M_state);

        // simple unpacking of supplied arguments into a tuple
        auto cat = [](auto ...args) {
            return std::tuple_cat(args...);
        };

        // retrieve a value at a specific index coerced into a specific type
        auto collect = [&](auto _idx, auto *_tp) {
            typedef remove_pointer(_tp) element_type;
            slw::reference ref { M_state, _idx() };
            return branch_on_same<element_type, slw::reference, element_type>::resolve(
                [&]() { return std::move(ref); },
                [&]() { return slw::as<element_type>(ref); }
            );
        };

        // iterate through all parameters and collect their values
        //  type is a tuple of all the different values
        auto arguments = range::type_range_t<Fp, Rp...>::collect(cat, collect);

        // make sure our stack is clear
        slw::clear(M_state);

        // unpack all arguments as actual parameters into the call to M_functor
        slw::range::apply<sizeof...(Rp) + 1>(
            M_functor,
            arguments
        );
    }

    void operator ()(Fp arg0, Rp ...args)
    { functor(arg0, args...);
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

    explicit callable_t(slw::shared_state state, functional_type fn_t)
        : M_state(fn_t)
        , M_functor(fn_t)
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
    typedef slw::functional<void, A...> functional_type;
    typedef typename functional_type::functional_type function_type;

    explicit callable_t(slw::shared_state state, function_type fn)
        : M_state(state)
        , M_functor(state, fn)
    {
    }

    explicit callable_t(slw::shared_state state, functional_type fn_t)
        : M_state(state)
        , M_functor(fn_t)
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

template<typename ...Args>
void prepare_call(slw::reference fn, Args ...args)
{
    slw::shared_state &state = fn.get_state();

    if (slw::TFUNCTION != fn.type())
        throw slw::unexpected_argument_type(state, fn.type(), slw::TFUNCTION);

    // retrieve a value at a specific index coerced into a specific type
    auto transform = [&](auto, auto, auto value) -> void {
        slw::push(state, value);
    };

    slw::range::type_range_t<Args...>::each(
        transform, args...
    );
}

template<typename ...Args>
slw::reference make_call(slw::reference fn, Args ... args)
{
    prepare_call(fn, args...);
    return fn();
}

} /*ns slw*/

#endif // FUNCTIONAL_HPP
