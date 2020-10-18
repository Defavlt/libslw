#include "catch.hpp"
#include "lua.hpp"
#include "slw/variant/variant.hpp"
#include "slw/variant/factory.hpp"
#include "slw/reference.hpp"
#include "slw/state.hpp"

#include <iostream>
#include <string>

using namespace slw;

#define a_str_value "Hello, world!"

SCENARIO( "variant", "[variant]" ) {
    GIVEN( "A state and a few variants" ) {
        shared_state state = make_state();
        open_libs(state);

        const char *script =
            "a_number = 42\n"
            "a_string = \"" a_str_value "\"\n"
            "person = {}\n"
            "person.a_string = a_string\n"
            "person.a_number = a_number\n"
            "person.has = {}\n"
            "person.has.towels = 42\n"
            "person.is = {}\n"
            "person.is.safe = (person.has.towels == 42)\n";

        // for convienience
        lua_State *L = state.get();
        luaL_dostring(L, script);

        REQUIRE(!lua_gettop(L));

        number a_number { state, "a_number" };
        string a_string { state, "a_string" };

        number person_a_number_0 { state, "person.a_number" };
        REQUIRE(42 == *person_a_number_0);
        REQUIRE(!lua_gettop(L));

        number person_a_number_1(state, person_a_number_0.get());
        number person_a_number_2(state, "person.a_number");

        REQUIRE(person_a_number_0 == person_a_number_1);
        REQUIRE(person_a_number_1 == person_a_number_2);
        REQUIRE(person_a_number_2 == person_a_number_0);

        string person_a_string { state, "person.a_string" };
        REQUIRE(a_str_value == *person_a_string);
        REQUIRE(!lua_gettop(L));

        integer towels { state, "person.has.towels" };
        REQUIRE(42 == *towels);
        REQUIRE(!lua_gettop(L));

        table person { state, "person" };
        REQUIRE(!lua_gettop(L));

        THEN( "the stack should be empty, and native and interpreted value should match" ) {
            REQUIRE( 42.0 == *person_a_number_0 );
            REQUIRE(!lua_gettop(L));
        }

        THEN( "(timing, use with -d yes) 1000000 tests on integer (slw::int_t) assignment is done" ) {
            integer int_timing = make_variant(state, (slw::int_t)-1);
            for (int i = 0; i < 1000000; ++i) {
                integer::expected_value_type i_p = i * i;
                int_timing = i_p;
                REQUIRE(i_p == *int_timing);
            }

            REQUIRE(!lua_gettop(L));
        }

        THEN( "(timing, use with -d yes) 1000000 tests on integer (slw::int_t) assignment is done" ) {
            number timing = make_variant(state, 1.0);
            for (int i = 0; i < 1000000; ++i) {
                number::expected_value_type i_p = i * i;
                timing = i_p;
                REQUIRE(i_p == *timing);
            }

            REQUIRE(!lua_gettop(L));
        }

        THEN( "(timing, use with -d yes) 1000000 tests on string (slw::string_t) assignment is done" ) {
            string timing = make_variant<slw::string_t>(state, "Hello, world!");
            for (int i = 0; i < 1000000; ++i) {
                string::expected_value_type ns = "helllo" + std::to_string(i * i);
                timing = ns;
                REQUIRE(ns == *timing);
            }

            REQUIRE(!lua_gettop(L));
        }

        // table doesn't make sense to run timings on
        // uint, bool is the same, or less than int

        REQUIRE(!lua_gettop(L));
        REQUIRE( (person_a_string == a_str_value) );
        REQUIRE(!lua_gettop(L));
        REQUIRE(!lua_gettop(L));
        REQUIRE( (towels == 42 ) );
        REQUIRE(!lua_gettop(L));

        REQUIRE(!lua_gettop(L));
        table tbl_has = person.get<table_t>("has");
        REQUIRE(!lua_gettop(L));
        integer tbl_towels = tbl_has.get<int_t>("towels");

        WHEN( "an inequality comparison between table variants is made" ) {
            REQUIRE( (person != tbl_has) );
            REQUIRE(!lua_gettop(L));
        }

        WHEN( "an equality comparison between table variants is made" ) {
            REQUIRE( (tbl_has == person.get<table_t>("has")) );
            REQUIRE(!lua_gettop(L));
        }

        WHEN ( "a table value is retrieved, and compared" ) {
            REQUIRE( (tbl_has.get<slw::number_t>("towels") == 42.0) );
            REQUIRE(!lua_gettop(L));
            REQUIRE( (tbl_has.get<slw::int_t>("towels") == *tbl_towels) );
            REQUIRE(!lua_gettop(L));
        }
    }
}
