#include "slw/reference.hpp"
#include "slw/get_field.hpp"
#include "slw/utility.hpp"
#include <lua.hpp>

#include <string>
#include <iostream>

void release(slw::shared_state &state, slw::int_t &ref)
{
    switch (ref) {
    case LUA_NOREF:
    case LUA_REFNIL:
        return;

    default:
        luaL_unref(state.get(), slw::internal::indexes::registry, ref);
    }
}

slw::int_t lock(slw::shared_state &state, const slw::string_t &name, slw::int_t idx)
{
    slw::internal::touch_path(state.get(), name, idx);
    return luaL_ref(state.get(), slw::internal::indexes::registry);
}

slw::reference::~reference()
{
    release(M_state, M_ref);
}

slw::reference::reference(slw::shared_state state)
    : M_state(state)
    , M_ref(LUA_REFNIL)
{
    if (lua_gettop(M_state.get()))
        M_ref = luaL_ref(state.get(), slw::internal::indexes::registry);
}

slw::reference::reference(slw::shared_state state, slw::int_t idx)
    : M_state(state)
    , M_ref(LUA_REFNIL)
{
    lua_pushvalue(state.get(), idx);
    M_ref = luaL_ref(state.get(), slw::internal::indexes::registry);
}

slw::reference::reference(slw::shared_state state, const slw::string_t &name)
    : M_state(state)
    , M_ref(lock(state, name, slw::internal::indexes::globals))
{
}

slw::reference::reference(slw::shared_state state, slw::int_t idx, const slw::string_t &name)
    : M_state(state)
    , M_ref(lock(state, name, idx))
{
}

slw::reference::reference(slw::shared_state state, slw::reference &t, const slw::string_t &k)
    : M_state(state)
    , M_ref(LUA_REFNIL)
{
    if (t.valid()) {
        auto m = bind(
            [&]() { t.push(); },
            [&]() { lua_pop(M_state.get(), 1); }
        );

        slw::push(state);
        lua_setfield(M_state.get(), -2, k.c_str());
        M_ref = luaL_ref(state.get(), slw::internal::indexes::registry);
    }
}

slw::reference::reference(const slw::reference &rhs)
{
    // an empty M_state means this is a 'null' reference
    if (M_state.use_count())
        release(M_state, M_ref);

    M_state = rhs.M_state;
    // borrow the other reference for a moment,
    //  and create a new reference
    M_ref = rhs.M_ref;
    push();
    M_ref = LUA_REFNIL;
    assign();
}

slw::reference &slw::reference::operator =(slw::reference rhs)
{
    M_state = rhs.M_state;
    rhs.push();
    assign();
    return *this;
}

void slw::reference::push()
{
    lua_rawgeti(M_state.get(), slw::internal::indexes::registry, M_ref);
}

void slw::reference::assign()
{
    release(M_state, M_ref);
    M_ref = luaL_ref(M_state.get(), slw::internal::indexes::registry);
}

slw::reference slw::reference::operator [](const slw::string_t &path)
{
    push();
    return slw::reference {
        M_state,
        -1,
        path
    };
}

slw::reference slw::reference::operator [](const slw::int_t &i)
{
    auto m = bind(
        [&]() { push(); },
        [&]() { lua_pop(M_state.get(), 1); }
    );

    lua_rawgeti(M_state.get(), -1, i);
    return slw::reference {
        M_state
    };
}

slw::reference &slw::reference::operator =(slw::reference &ref)
{
    ref.push();
    assign();
    return *this;
}

bool slw::reference::valid()
{
    auto m = slw::bind(
        [&]() { push(); },
        [&]() { lua_pop(M_state.get(), 1); }
    );

    return (LUA_REFNIL != M_ref)
            && (LUA_NOREF != M_ref)
            && (LUA_TNIL != lua_type(M_state.get(), -1));
}

bool slw::reference::operator ==(const slw::reference &rhs) const
{
    slw::reference *p_rhs = const_cast<slw::reference *>(&rhs);
    slw::reference *p_lhs = const_cast<slw::reference *>(this);

    p_rhs->push();
    p_lhs->push();

    bool eq = lua_compare(p_lhs->M_state.get(), -1, -2, LUA_OPEQ);
    lua_pop(p_lhs->M_state.get(), 2);
    return eq;
}

bool slw::reference::operator !=(const slw::reference &rhs) const
{ return !(*this == rhs);
}

slw::shared_state &slw::reference::get_state()
{
    return M_state;
}

const slw::shared_state &slw::reference::get_state() const
{
    return M_state;
}

slw::type_e slw::reference::type()
{
    push();
    slw::type_e type = (slw::type_e)lua_type(M_state.get(), -1);
    lua_pop(M_state.get(), 1);
    return type;
}

slw::reference slw::make_reference(slw::shared_state state, const slw::string_t &name)
{
    return slw::reference { state, name };
}

#define ASSIGN_MT_IMPL(native_type)                                 \
void slw::reference::assign(const slw::string_t &&k, native_type v) \
{                                                                   \
    push();                                                         \
    slw::push(M_state, v);                                          \
    lua_setfield(M_state.get(), -2, k.c_str());                     \
    lua_pop(M_state.get(), 1);                                      \
}

ASSIGN_MT_IMPL(slw::number_t)
ASSIGN_MT_IMPL(slw::uint_t)
ASSIGN_MT_IMPL(slw::int_t)
ASSIGN_MT_IMPL(const slw::string_t &&)
ASSIGN_MT_IMPL(slw::bool_t)

void slw::reference::assign(const slw::string_t &&k, slw::reference &v)
{
    push();
    v.push();
    lua_setfield(M_state.get(), -2, k.c_str());
    lua_pop(M_state.get(), 1);
}

#undef ASSIGN_MT_IMPL

#define PUSH_FREE_FN_IMPL(native_type, push_fn)                                \
template<>                                                                     \
void slw::push<native_type>(slw::shared_state &state, native_type value)  \
{ push_fn(state.get(), value);                                                 \
}

PUSH_FREE_FN_IMPL(slw::number_t, lua_pushnumber)
PUSH_FREE_FN_IMPL(slw::uint_t, lua_pushinteger)
PUSH_FREE_FN_IMPL(slw::int_t, lua_pushinteger)
PUSH_FREE_FN_IMPL(slw::bool_t, lua_pushboolean)

#undef PUSH_FREE_FN_IMPL

void slw::push(slw::shared_state &state, const slw::string_t &value)
{ lua_pushstring(state.get(), value.c_str());
}

template<>
void slw::push(slw::shared_state &state, slw::reference &value)
{ value.push();
}

void slw::push(slw::shared_state &state)
{ lua_createtable(state.get(), 0, 0);
}
