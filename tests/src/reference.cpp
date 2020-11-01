#include "catch.hpp"
#include "lua.hpp"
#include "slw/state.hpp"
#include "slw/reference.hpp"

using namespace slw;

SCENARIO( "reference", "[reference]" ) {
    GIVEN( "A state" ) {
        shared_state state = make_state();
        open_libs(state);

        lua_State *L = state.get();

#define TOWELS 42

        const char *script =
            "person = {}\n"
            "person.has = {}\n"
            "person.has.towels = 42\n"
            "person.has.pets = {}"
            "person.is = {}\n"
            "person.is.safe = (person.has.towels == 42)\n";

        luaL_dostring(state.get(), script);

        WHEN( "a object, in the state, is referenced" ) {
            luaL_dostring(state.get(), script);
        }
    }
}
