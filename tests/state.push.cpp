#include "catch.hpp"
#include "slw/state.hpp"

typedef std::string str;

SCENARIO( "state can be pushed", "[state]" ) {
  GIVEN( "A Lua State" ) {
    slw::State st;

    REQUIRE( !st.top() );

    WHEN( "values are pushed" ) {
      const int i_ovalue = 5;
      const str s_ovalue = "Hello, world!";

      st.push(i_ovalue);
      st.push(s_ovalue);

      THEN( "the size changes accordingly" ) {
        REQUIRE( 2 == st.top() );
      }
    }
  }
}
