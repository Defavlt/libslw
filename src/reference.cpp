#include "slw/reference.hpp"
#include "slw/get_field.hpp"
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
        t.push();
        lua_createtable(M_state.get(), 0, 0);
        lua_setfield(M_state.get(), -2, k.c_str());
        lua_pop(M_state.get(), 1);
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
    push();
    lua_rawgeti(M_state.get(), -1, i);
    lua_remove(M_state.get(), -2);
    lua_pop(M_state.get(), 1);

    return slw::reference { M_state, -1 };
}

bool slw::reference::valid()
{
    push();
    bool is_nil =
            (LUA_REFNIL != M_ref)
            && (LUA_NOREF != M_ref)
            && (LUA_TNIL != lua_type(M_state.get(), -1));
    lua_pop(M_state.get(), 1);
    return is_nil;
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
