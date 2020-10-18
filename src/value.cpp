#include "slw/value.hpp"
#include "slw/utility.hpp"
#include "lua.hpp"

slw::value::value_t(slw::shared_state state, const slw::string_t &t, const slw::string_t &k)
    : M_state(state)
    , M_ref(state, t)
    , M_idx(k)
{
}

slw::value::value_t(slw::shared_state state, slw::int_t t, const slw::string_t &k)
    : M_state(state)
    , M_ref(state, t)
    , M_idx(k)
{
}

slw::value::value_t(const slw::value &rhs)
    : M_state(rhs.M_state)
    , M_ref(rhs.M_ref)
    , M_idx(rhs.M_idx)
{
}

slw::array_value::value_t(slw::shared_state state, const slw::string_t &t, slw::int_t k)
    : M_state(state)
    , M_ref(state, t)
    , M_idx(k)
{
}

slw::array_value::value_t(slw::shared_state state, slw::int_t t, slw::int_t k)
    : M_state(state)
    , M_ref(state, t)
    , M_idx(k)
{
}

slw::value &slw::value::operator =(slw::reference ref)
{
    M_ref.push();
    ref.push();
    lua_setfield(M_state.get(), -2, M_idx.c_str());
    return *this;
}

slw::value &slw::value::operator =(slw::value rhs)
{
    std::swap(M_state, rhs.M_state);
    std::swap(M_ref, rhs.M_ref);
    std::swap(M_idx, rhs.M_idx);
    return *this;
}

//slw::array_value::value_t(slw::shared_state state, slw::reference &t, slw::int_t k)
//    : M_state(state)
//    , M_ref(t)
//    , M_idx(k)
//{

//}

slw::array_value::value_t(const slw::array_value &rhs)
    : M_state(rhs.M_state)
    , M_ref(rhs.M_ref)
    , M_idx(rhs.M_idx)
{
}

slw::array_value &slw::array_value::operator =(slw::reference ref)
{
    M_ref.push();
    ref.push();
    lua_rawseti(M_state.get(), -2, M_idx);
    return *this;
}

slw::array_value &slw::array_value::operator =(slw::array_value rhs)
{
    std::swap(M_state, rhs.M_state);
    std::swap(M_ref, rhs.M_ref);
    std::swap(M_idx, rhs.M_idx);
    return *this;
}

void slw::value::assign()
{
    M_ref.push();
    lua_pushnil(M_state.get());
    lua_copy(M_state.get(), -3, -1);
    /*
     * -1: value
     * -2: ref
     * -3: nil
     * */
    lua_setfield(M_state.get(), -2, M_idx.c_str());
    lua_pop(M_state.get(), 2);
}

void slw::value::push()
{
    M_ref.push();
    lua_getfield(M_state.get(), -1, M_idx.c_str());
    // remove M_ref
    lua_remove(M_state.get(), lua_absindex(M_state.get(), -2));
}

void slw::array_value::assign()
{
    M_ref.push();
    lua_pushnil(M_state.get());
    lua_copy(M_state.get(), -3, -1);
    lua_rawseti(M_state.get(), -2, M_idx);
    lua_pop(M_state.get(), 1);
}

void slw::array_value::push()
{
    M_ref.push();
    lua_rawgeti(M_state.get(), -1, M_idx);
    // remove M_ref
    lua_remove(M_state.get(), lua_absindex(M_state.get(), -2));
}
