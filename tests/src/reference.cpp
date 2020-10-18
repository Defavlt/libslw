#include "catch.hpp"
#include "lua.hpp"
#include "slw/state.hpp"
#include "slw/reference.hpp"
#include "slw/variant/factory.hpp"

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

            slw::reference person(state, "person");
            REQUIRE(0 == lua_gettop(L));

            reference has = person["has"];
            REQUIRE(0 == lua_gettop(L));

            number towels = has["towels"];
            REQUIRE(0 == lua_gettop(L));

            // Remember, the assignment operator is no more than
            // a copy construction, and two swaps.
            number towels_2 = make_variant(state, *towels);
            REQUIRE(0 == lua_gettop(L));

            person.push();
            REQUIRE(1 == lua_gettop(L));
            REQUIRE(lua_istable(L, -1));
            lua_pop(L, 1);

            has.push();
            REQUIRE(1 == lua_gettop(L));
            REQUIRE(lua_istable(L, -1));
            lua_pop(L, 1);

            towels.get().push();
            REQUIRE(1 == lua_gettop(L));
            REQUIRE(lua_isnumber(L, -1));
            REQUIRE(TOWELS == lua_tonumber(L, -1));
            lua_pop(L, 1);

            towels_2.get().push();
            REQUIRE(1 == lua_gettop(L));
            REQUIRE(lua_isnumber(L, -1));
            REQUIRE(TOWELS == lua_tonumber(L, -1));
            lua_pop(L, 1);

            table pets = person["has"]["pets"];
            string s { state };
#define NO_DOGS_HERE_NO "no dogs here, nonono"

            s = NO_DOGS_HERE_NO;

            pets.set("dogs", s.get());
            REQUIRE(!lua_gettop(L));
            REQUIRE(NO_DOGS_HERE_NO == *s);

            s = person["has"]["pets"]["dogs"];
            REQUIRE(pets["dogs"] == person["has"]["pets"]["dogs"]);
        }
    }
}
