
#include <test.hpp>

using namespace pride;

TEST_CASE("uint128_t")
{
    SECTION("Construction")
    {
        SECTION("Standard")
        {
            uint128_t value(0x0123456789abcdefULL);
            const uint128_t original = value;

            REQUIRE(uint128_t() == 0);
            REQUIRE(value == original);
            REQUIRE(uint128_t(std::move(value)) == original);
            REQUIRE(value == 0);
        }

        SECTION("One")
        {
            REQUIRE(static_cast<bool>(uint128_t(true).hi)  == false);
            REQUIRE(static_cast<bool>(uint128_t(true).lo)  == true);
            REQUIRE(static_cast<bool>(uint128_t(false).hi) == false);
            REQUIRE(static_cast<bool>(uint128_t(false).lo) == false);

            REQUIRE(uint128_t((uint8_t)  0x01ULL).hi               == 0ULL);
            REQUIRE(uint128_t((uint16_t) 0x0123ULL).hi             == 0ULL);
            REQUIRE(uint128_t((uint32_t) 0x01234567ULL).hi         == 0ULL);
            REQUIRE(uint128_t((uint64_t) 0x0123456789abcdefULL).hi == 0ULL);

            REQUIRE(uint128_t((uint8_t)  0x01ULL).lo               == (uint8_t)  0x01ULL);
            REQUIRE(uint128_t((uint16_t) 0x0123ULL).lo             == (uint16_t) 0x0123ULL);
            REQUIRE(uint128_t((uint32_t) 0x01234567ULL).lo         == (uint32_t) 0x01234567ULL);
            REQUIRE(uint128_t((uint64_t) 0x0123456789abcdefULL).lo == (uint64_t) 0x0123456789abcdefULL);
        }

        SECTION("Two")
        {
            for(uint8_t hi = 0; hi < 2; hi++)
            {
                for(uint8_t lo = 0; lo < 2; lo++)
                {
                    const uint128_t val(hi, lo);
                    REQUIRE(val.hi == hi);
                    REQUIRE(val.lo == lo);
                }
            }

            REQUIRE(uint128_t((uint8_t)  0x01ULL,               (uint8_t)  0x01ULL).hi ==               (uint8_t)  0x01ULL);
            REQUIRE(uint128_t((uint16_t) 0x0123ULL,             (uint16_t) 0x0123ULL).hi ==             (uint16_t) 0x0123ULL);
            REQUIRE(uint128_t((uint32_t) 0x01234567ULL,         (uint32_t) 0x01234567ULL).hi ==         (uint32_t) 0x01234567ULL);
            REQUIRE(uint128_t((uint64_t) 0x0123456789abcdefULL, (uint64_t) 0x0123456789abcdefULL).hi == (uint64_t) 0x0123456789abcdefULL);

            REQUIRE(uint128_t((uint8_t)  0x01ULL,               (uint8_t)  0x01ULL).lo ==               (uint8_t)  0x01ULL);
            REQUIRE(uint128_t((uint16_t) 0x0123ULL,             (uint16_t) 0x0123ULL).lo ==             (uint16_t) 0x0123ULL);
            REQUIRE(uint128_t((uint32_t) 0x01234567ULL,         (uint32_t) 0x01234567ULL).lo ==         (uint32_t) 0x01234567ULL);
            REQUIRE(uint128_t((uint64_t) 0x0123456789abcdefULL, (uint64_t) 0x0123456789abcdefULL).lo == (uint64_t) 0x0123456789abcdefULL);
        }
    }

    SECTION("Assignment operator")
    {
        const uint128_t t_1   = true;
        const uint128_t f_1   = false;
        const uint128_t u8_1  = 0x01;
        const uint128_t u16_1 = 0x0123;
        const uint128_t u32_1 = 0x01234567;
        const uint128_t u64_1 = 0x0123456789abcdef;

        uint128_t t_2   = 0;
        uint128_t f_2   = 0;
        uint128_t u8_2  = 0;
        uint128_t u16_2 = 0;
        uint128_t u32_2 = 0;
        uint128_t u64_2 = 0;

        t_2 = t_1;
        f_2 = f_1;
        u8_2 = u8_1;
        u16_2 = u16_1;
        u32_2 = u32_1;
        u64_2 = u64_1;

        REQUIRE(t_1 == t_2);
        REQUIRE(f_1 == f_2);
        REQUIRE(u8_1 == u8_2);
        REQUIRE(u16_1 == u16_2);
        REQUIRE(u32_1 == u32_2);
        REQUIRE(u64_1 == u64_2);
    }

    SECTION("Type cast")
    {
        const uint128_t val(0xaaaaaaaaaaaaaaaaULL, 0xaaaaaaaaaaaaaaaaULL);

        REQUIRE(static_cast <bool>     (uint128_t(true)) ==          true);
        REQUIRE(static_cast <bool>     (uint128_t(false)) ==         false);
        REQUIRE(static_cast <uint8_t>  (val) ==           (uint8_t)  0xaaULL);
        REQUIRE(static_cast <uint16_t> (val) ==           (uint16_t) 0xaaaaULL);
        REQUIRE(static_cast <uint32_t> (val) ==           (uint32_t) 0xaaaaaaaaULL);
        REQUIRE(static_cast <uint64_t> (val) ==           (uint64_t) 0xaaaaaaaaaaaaaaaaULL);
    }
}
