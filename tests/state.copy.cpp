#include "catch.hpp"
#include "slw/state.hpp"
#include "slw/types.hpp"

SCENARIO( "state", "[state]" ) {
  GIVEN( "A Lua State" ) {
    slw::State st0;
    slw::State st1 = st0;

    REQUIRE( !st0.top() );
    REQUIRE( !st1.top() );

    REQUIRE( st0.state == st1.state );

    st0.push(5);
    REQUIRE( st0.top() == st1.top() );
  }
}
