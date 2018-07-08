#include <iostream>
#include <string>

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "lua.hpp"

#include "slw/call.hpp"
#include "slw/field.hpp"
#include "slw/types.hpp"

int main(int argc, char **argv)
{
    return Catch::Session().run(argc, argv);
}
