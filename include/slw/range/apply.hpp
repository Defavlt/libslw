#ifndef SLW_RANGE_APPLY_HPP
#define SLW_RANGE_APPLY_HPP

namespace slw {
namespace range {

template<int ...>
struct seq {
};

template<int N, int ...S>
struct gens : gens<N - 1, N - 1, S...> {
};

template<int ...S>
struct gens<0, S...> {
    typedef seq<S...> type;
};

template<typename F, typename A, int ...S>
auto dispatch(F fn, A args, seq<S...>)
{
    return fn(std::get<S>(args) ...);
}

template<int S, typename F, typename T>
auto apply(F fn, T args)
{
    return dispatch(fn, args, typename gens<S>::type());
}

}/*ns range*/
}/*ns slw*/

#endif/*SLW_RANGE_APPLY_HPP*/
