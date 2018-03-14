
#include <test.hpp>

using namespace pride;

TEST_CASE("Scope Guard")
{
    int something = 0;
    {
        defer
        {
            something = 10;
        };

        CHECK(something == 0);
    }
    CHECK(something == 10);
}
