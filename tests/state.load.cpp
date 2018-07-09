#include "catch.hpp"
#include "slw/state.hpp"

SCENARIO( "state can load valid Lua", "[state]" ) {
  GIVEN( "A Lua State" ) {
    slw::State st;

    REQUIRE( st.load("function hw () print('Hello, world!') end") );
  }
}
