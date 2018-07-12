#include "catch.hpp"
#include "slw/state.hpp"
#include "slw/types.hpp"

typedef std::string str;

SCENARIO( "state can be popped", "[state]" ) {
  GIVEN( "A Lua State" ) {
    slw::State st;

    REQUIRE( !st.top() );

    WHEN( "values are pushed, then popped" ) {
      const int i_ovalue = 5;
      const str s_ovalue = "Hello, world!";

      int i_value = 0;
      str s_value = "";

      st.push(i_ovalue);
      st.push(s_ovalue);

      THEN( "the size changes accordingly, and the popped values assigned" ) {
        REQUIRE( st.pop(s_value) );
        REQUIRE( s_ovalue == s_value );
        REQUIRE( 1 == st.top() );

        REQUIRE( st.pop(i_value) );
        REQUIRE( i_ovalue == i_value );
        REQUIRE( !st.top() );
      }
    }
  }
}
