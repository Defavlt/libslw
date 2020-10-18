#ifndef SLW_RANGE_TYPE_RANGE_HPP
#define SLW_RANGE_TYPE_RANGE_HPP

#include <type_traits>

namespace slw {
namespace range {

template<int, int, typename ...>
////////////////////////////////////////////////////////////
/// \brief Base case
////////////////////////////////////////////////////////////
struct type_range {
};

template<int I, int C, typename H>
////////////////////////////////////////////////////////////
/// \brief Type validation container, for a single item.
////////////////////////////////////////////////////////////
struct type_range<I, C, H> {
    typedef H                               value_type;
    typedef H                               *ptr_type;
    typedef std::integral_constant<int, I>  index_type;

    static constexpr ptr_type type = nullptr;

    template<typename V, typename ...A>
    ////////////////////////////////////////////////////////////
    /// \brief Validate a type, using supplied functor \a fn.
    /// \param fn The functor testing the type.
    /// Must have the signature bool(int, H, ...)
    /// \param args Any user-supplied arguments needed for functor \a fn.
    /// \return The result of the call to \a fn
    ////////////////////////////////////////////////////////////
    static inline bool all(V fn, A &&...args)
    {
        return fn(index_type {}, type, std::forward<A>(args)...);
    }

    template<typename V, typename ...A>
    static inline bool any(V fn, A &&...args)
    {
        return all(fn, args...);
    }

    template<typename Cat, typename V, typename ...A>
    static inline auto collect(Cat, V fn, A &&...args)
    {
        return std::make_tuple(fn(index_type {}, type, args...));
    }
};

template<int I, int C, typename H, typename ...T>
////////////////////////////////////////////////////////////
/// \brief Iterate a range of types, using a supplied functor.
/// \param I The index to start at
/// \param C The change, to the index, applied for each type.
////////////////////////////////////////////////////////////
struct type_range<I, C, H, T...> {
    typedef type_range<I, C, H>           head_type;
    typedef type_range<I + C, C, T...>    tail_type;
    typedef std::tuple<H, T...>             tuple_type;

    template<typename V, typename ...A>
    ////////////////////////////////////////////////////////////
    /// \brief Validate *all* types, using supplied functor \a fn.
    /// \param fn The functor testing the type.
    /// Must have the signature bool(int, H, ...)
    /// \param args Any user-supplied arguments needed for functor \a fn.
    /// \return The result of the call to \a fn && ...
    ////////////////////////////////////////////////////////////
    static inline bool all(V fn, A &&...args)
    {
        return head_type::all(fn, args...)
                && tail_type::all(fn, args...);
    }

    template<typename V, typename ...A>
    ////////////////////////////////////////////////////////////
    /// \brief Validate *any* type, using supplied functor \a fn.
    /// \param fn The functor testing the type.
    /// Must have the signature bool(int, H, ...)
    /// \param args Any user-supplied arguments needed for functor \a fn.
    /// \return The result of the call to \a fn || ...
    ////////////////////////////////////////////////////////////
    static inline bool any(V fn, A &&...args)
    {
        return head_type::any(fn, args...)
                || tail_type::any(fn, args...);
    }

    template<typename Cat, typename Collector, typename ...A>
    ////////////////////////////////////////////////////////////
    /// \brief Collect the types, using supplied functor \a fn.
    /// \param fn The functor transforming the type
    /// \param args Any user-supplied arguments needed for functor \a fn.
    /// \return The resulting type
    ////////////////////////////////////////////////////////////
    static inline auto collect(Cat cat, Collector fn, A &&...args)
    {
        return cat(
            head_type::collect(cat, fn, args...),
            tail_type::collect(cat, fn, args...)
        );
    }
};

template<typename ...T>
////////////////////////////////////////////////////////////
/// \brief \typedef validate Validate a sequence of types.
/// \see slw::validate::type_range
////////////////////////////////////////////////////////////
using type_range_t = type_range<1, 1, T...>;

}/*ns range*/
}/*ns slw*/

#endif/*SLW_RANGE_TYPE_RANGE_HPP*/

////////////////////////////////////////////////////////////
/// \brief Acquire the type of the second argument to the functor supplied
/// by \see slw::validate::type_range::all, \see slw::validate::type_range::any
////////////////////////////////////////////////////////////
#define remove_pointer(x) typename std::remove_pointer<decltype(x)>::type

////////////////////////////////////////////////////////////
/// \class slw::validate::range
/// \ingroup utility
///
/// slw::validate::type_range is a type sequence, where each element
/// of the sequence is a *type*.
///
/// Usage example:
/// \code
/// // the second argument is a null ptr of the element type
/// auto val = [](int _idx, auto *_tp, std::string, int) -> bool {
///     // use macro, if you wish, for acquiring the type of the pointer
///     typedef remove_pointer(_tp) element_type;
///     /* ... */
/// };
///
/// // you can supply your own values as well
/// if (slw::validate::range<float, double, int>::all(val, "", 5))
///     /* if every call to val evaluates to true, you'd end up here */
///
/// \endcode
///
////////////////////////////////////////////////////////////
