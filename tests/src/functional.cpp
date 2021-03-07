#include "catch.hpp"
#include <iostream>
#include <string>

#include "lua.hpp"
#include "slw/types.hpp"
#include "slw/state.hpp"
#include "slw/functional.hpp"

using namespace slw;

#define STRING_0 "Hello, world: 0"
#define STRING_1 "Hello, world: 1"
#define INT_0    0xDEADBEEF
#define INT_1    0xDEADB00B

class MemberTest {
public:
    void call_void_str(const slw::string_t &str) {
        std::cout << "MemberTest::call_void(" << str << ")" << std::endl;
    }

    void call_void_void() {
        std::cout << "MemberTest::call_void_void()" << std::endl;
    }

    slw::int_t call_int_str(const slw::string_t &str) {
        std::cout << "MemberTest::call_int_str(" << str << ")" << std::endl;
        return 1;
    }

    slw::int_t call_int_void() {
        std::cout << "MemberTest::call_int_void(" << ")" << std::endl;
        return 1;
    }

    void call_void_str2_ref(const slw::string_t &, const slw::string_t &, slw::reference)
    {
    }
};

#define type_fn(type, state, fn) \
    type::functional_type { state, fn }

#define build_callable(type, name, state, fn) \
    type name { \
        state,  \
        type_fn(type, state, fn) \
    }


SCENARIO( "functional", "[functional]" ) {
    GIVEN( "functor objects of different types" ) {

        slw::shared_state state = make_state();
        MemberTest instance;

        typedef callable_t<void, slw::string_t> mem_type_void_str;
        typedef callable_t<void> mem_type_void_void;
        typedef callable_t<slw::int_t, slw::string_t> mem_type_int_str;
        typedef callable_t<slw::int_t> mem_type_int_void;

        typedef callable_t<void, slw::string_t, slw::string_t, slw::reference> mem_type_void_str2_ref;

        using namespace std::placeholders;

        mem_type_void_str2_ref mem_call_void_str2_ref {
            state,
            std::bind(&MemberTest::call_void_str2_ref, instance, _1, _2, _3)
        };

        mem_type_void_str mem_call_void_str {
            state,
            std::bind(&MemberTest::call_void_str, instance, _1)
        };

        mem_type_void_void mem_call_void_void {
            state,
            std::bind(&MemberTest::call_void_void, instance)
        };

        mem_type_int_str mem_call_int_str {
            state,
            std::bind(&MemberTest::call_int_str, instance, _1)
        };

        mem_type_int_void mem_call_int_void {
            state,
            std::bind(&MemberTest::call_int_void, instance)
        };

        typedef callable_t<slw::int_t, slw::string_t>       fn_type0;
        typedef callable_t<slw::string_t, slw::string_t>    fn_type1;
        typedef callable_t<void> fn_type_void;

        fn_type0 call_0 {
            state,
            [](slw::string_t arg0) -> slw::int_t {
                return INT_0;
            }
        };

        fn_type1 call_1 {
            state,
            [](slw::string_t arg0) -> slw::string_t {
                slw::string_t s = "Hello, " + arg0 + "!";
                std::cout << s << std::endl;
                return STRING_1;
            }
        };

        fn_type_void {
            state,
            []() {
                std::cout << "fn_type_void callback" << std::endl;
            }
        };

        const char *src_tbl = "callables = {}";
        luaL_dostring(state.get(), src_tbl);

        reference callables { state, "callables" };
        reference ref_0 = make_callable(state, call_0);
        reference ref_1 = make_callable(state, call_1);

        callables.assign("fn_type0", ref_0);
        callables.assign("fn_type1", ref_1);

        const char *src_call =
                "callables.ret0 = callables.fn_type0(\"Hello, world!\")\n"
                "callables.ret1 = callables.fn_type1(\"Hello, world!\")\n"
                "print(type(callables.fn_type0))\n"
                "print(type(callables.fn_type1))\n";

        THEN("") {
            luaL_dostring(state.get(), src_call);
            slw::reference ret_0 { state, "callables.ret0" };
            slw::reference ret_1 { state, "callables.ret1" };

            REQUIRE(slw::is<slw::int_t>(ret_0));
            REQUIRE(slw::is<slw::string_t>(ret_1));

            REQUIRE(INT_0 == slw::as<slw::int_t>(ret_0));
            REQUIRE(STRING_1 == slw::as<slw::string_t>(ret_1));
        }
    }
}
