#include "catch.hpp"
#include "slw/state.hpp"
#include "lua.hpp"

using namespace slw;

SCENARIO( "field", "[field]" ) {
    WHEN( "A state is created, and deleted" ) {
        slw::shared_state state;
        REQUIRE_NOTHROW(state = make_state());
        REQUIRE(!lua_gettop(state.get()));
    }

    GIVEN( "A state" ) {
        shared_state state = make_state();

        WHEN ( "libraries are opened" ) {
            REQUIRE_NOTHROW(open_libs(state));
            REQUIRE(!lua_gettop(state.get()));
        }
    }
}
