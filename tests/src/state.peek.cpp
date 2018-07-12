#include "catch.hpp"
#include "slw/state.hpp"
#include "slw/types.hpp"

typedef std::string str;

SCENARIO( "state can be peeked", "[state]" ) {
  GIVEN( "A Lua State" ) {
    slw::State st;

    REQUIRE( !st.top() );

    WHEN( "values are pushed, then peeked" ) {
      const int i_ovalue = 5;
      const str s_ovalue = "Hello, world!";

      int i_value = 0;
      str s_value = "";

      st.push(i_ovalue);
      st.push(s_ovalue);

      THEN( "the size changes accordingly" ) {
        REQUIRE( 2 == st.top() );
      }

      THEN( "the values can be peeked" ) {
        REQUIRE( st.peek(s_value) );
        REQUIRE( s_ovalue == s_value );
        st.pop();

        REQUIRE( 1 == st.top() );
        REQUIRE( st.peek(i_value) );
        REQUIRE( i_ovalue == i_value );
      }
    }
  }
}
