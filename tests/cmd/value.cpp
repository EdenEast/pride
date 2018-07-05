
#include <test.hpp>
#include <string>
#include <utility>

TEST_CASE("cmd::vale")
{
    SECTION("Integer parsing")
    {

        SECTION("Parse valid hex values")
        {
            std::vector<std::pair<std::string, uint64_t>> pairs = {
                {"0x00000000", 0x00000000L}, {"0xFfFfFfFf", 0xFFFFFFFF},
                {"0xf4d4b551", 0xf4d4b551L}, {"0x7f6a0dbb", 0x7f6a0dbbL},
                {"0xc1611dab", 0xc1611dabL}, {"0xacbcf940", 0xacbcf940L},
                {"0x1adad47d", 0x1adad47dL}, {"0x4369e96a", 0x4369e96aL}
            };

            uint64_t result;
            for (auto p : pairs)
            {
                pride::cmd::detail::parse_value(p.first, result);
                CHECK(p.second == result);
            }
        }

        SECTION("Parse invalid hex values")
        {
            std::vector<std::string> values = { "0acbcf940", "cx1adad47d", "0x4369je96a" };

            uint64_t result;
            for (auto& v : values)
                REQUIRE_THROWS_AS(pride::cmd::detail::parse_value(v, result), pride::cmd::detail::argument_incorrect_type_t);
        }

        SECTION("Parse valid decimal values")
        {
            std::vector<std::pair<std::string, uint64_t>> pairs = {
                {"1234567890", 1234567890}, {"8732712", 8732712}, {"1", 1}, {"90", 90}, {"0", 0}, {"00000", 0}
            };

            uint64_t result;
            for (auto p : pairs)
            {
                pride::cmd::detail::parse_value(p.first, result);
                CHECK(p.second == result);
            }
        }

        SECTION("Parse invalid decimal values")
        {
            std::vector<std::string> values = {
                "0123", "4567hd7", "066h2", "2345678l8"
            };

            uint64_t result;
            for (auto& v : values)
                REQUIRE_THROWS_AS(pride::cmd::detail::parse_value(v, result), pride::cmd::detail::argument_incorrect_type_t);
        }
    } // SECTION("Integer parsing")
}
