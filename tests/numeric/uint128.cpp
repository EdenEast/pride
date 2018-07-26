
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

    SECTION("Bitwise operator")
    {
        SECTION("And operator")
        {
            SECTION("Internal")
            {
                uint128_t t  ((bool)     true);
                uint128_t f  ((bool)     false);
                uint128_t u8 ((uint8_t)  0xaaULL);
                uint128_t u16((uint16_t) 0xaaaaULL);
                uint128_t u32((uint32_t) 0xaaaaaaaaULL);
                uint128_t u64((uint64_t) 0xaaaaaaaaaaaaaaaaULL);

                const uint128_t val(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f0ULL);

                REQUIRE((t   &  val) == uint128_t(0));
                REQUIRE((f   &  val) == uint128_t(0));
                REQUIRE((u8  &  val) == uint128_t(0xa0ULL));
                REQUIRE((u16 &  val) == uint128_t(0xa0a0ULL));
                REQUIRE((u32 &  val) == uint128_t(0xa0a0a0a0ULL));
                REQUIRE((u64 &  val) == uint128_t(0xa0a0a0a0a0a0a0a0ULL));

                REQUIRE((t   &= val) == uint128_t(0x0ULL));
                REQUIRE((f   &= val) == uint128_t(0x0ULL));
                REQUIRE((u8  &= val) == uint128_t(0xa0ULL));
                REQUIRE((u16 &= val) == uint128_t(0xa0a0ULL));
                REQUIRE((u32 &= val) == uint128_t(0xa0a0a0a0ULL));
                REQUIRE((u64 &= val) == uint128_t(0xa0a0a0a0a0a0a0a0ULL));
            }

            SECTION("External")
            {
                uint128_t t  ((bool)     true);
                uint128_t f  ((bool)     false);
                uint128_t u8 ((uint8_t)  0xaaULL);
                uint128_t u16((uint16_t) 0xaaaaULL);
                uint128_t u32((uint32_t) 0xaaaaaaaaULL);
                uint128_t u64((uint64_t) 0xaaaaaaaaaaaaaaaaULL);

                const uint128_t val(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f0ULL);

                REQUIRE((t   &  val) == uint128_t(0));
                REQUIRE((f   &  val) == uint128_t(0));
                REQUIRE((u8  &  val) == uint128_t(0xa0ULL));
                REQUIRE((u16 &  val) == uint128_t(0xa0a0ULL));
                REQUIRE((u32 &  val) == uint128_t(0xa0a0a0a0ULL));
                REQUIRE((u64 &  val) == uint128_t(0xa0a0a0a0a0a0a0a0ULL));

                REQUIRE((t   &= val) == uint128_t(0x0ULL));
                REQUIRE((f   &= val) == uint128_t(0x0ULL));
                REQUIRE((u8  &= val) == uint128_t(0xa0ULL));
                REQUIRE((u16 &= val) == uint128_t(0xa0a0ULL));
                REQUIRE((u32 &= val) == uint128_t(0xa0a0a0a0ULL));
                REQUIRE((u64 &= val) == uint128_t(0xa0a0a0a0a0a0a0a0ULL));

                REQUIRE((uint128_t() & val) == 0);
            }
        }

        SECTION("Or operator")
        {
            SECTION("Internal")
            {
                uint128_t t  ((bool)     true);
                uint128_t f  ((bool)     false);
                uint128_t u8 ((uint8_t)  0xaaULL);
                uint128_t u16((uint16_t) 0xaaaaULL);
                uint128_t u32((uint32_t) 0xaaaaaaaaULL);
                uint128_t u64((uint64_t) 0xaaaaaaaaaaaaaaaaULL);

                const uint128_t val(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f0ULL);

                REQUIRE((t   |  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f1ULL));
                REQUIRE((f   |  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f0ULL));
                REQUIRE((u8  |  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0faULL));
                REQUIRE((u16 |  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0fafaULL));
                REQUIRE((u32 |  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0fafafafaULL));
                REQUIRE((u64 |  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xfafafafafafafafaULL));

                REQUIRE((t   |= val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f1ULL));
                REQUIRE((f   |= val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f0ULL));
                REQUIRE((u8  |= val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0faULL));
                REQUIRE((u16 |= val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0fafaULL));
                REQUIRE((u32 |= val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0fafafafaULL));
                REQUIRE((u64 |= val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xfafafafafafafafaULL));

                REQUIRE((uint128_t() | val) == val);
            }

            SECTION("External")
            {
                bool     t   = true;
                bool     f   = false;
                uint8_t  u8  = 0xaa;
                uint16_t u16 = 0xaaaa;
                uint32_t u32 = 0xaaaaaaaaULL;
                uint64_t u64 = 0xaaaaaaaaaaaaaaaaULL;

                const uint128_t val(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f0ULL);

                REQUIRE((t   |  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f1ULL));
                REQUIRE((f   |  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f0ULL));
                REQUIRE((u8  |  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0faULL));
                REQUIRE((u16 |  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0fafaULL));
                REQUIRE((u32 |  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0fafafafaULL));
                REQUIRE((u64 |  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xfafafafafafafafaULL));

                REQUIRE((t   |= val) == true);
                REQUIRE((f   |= val) == true);
                REQUIRE((u8  |= val) == (uint8_t)  0xfaULL);
                REQUIRE((u16 |= val) == (uint16_t) 0xfafaULL);
                REQUIRE((u32 |= val) == (uint32_t) 0xfafafafaULL);
                REQUIRE((u64 |= val) == (uint64_t) 0xfafafafafafafafaULL);

                // zero
                REQUIRE((uint128_t() | val) == val);
            }
        }

        SECTION("Xor operator")
        {
            SECTION("Internal")
            {
                uint128_t t  ((bool)     true);
                uint128_t f  ((bool)     false);
                uint128_t u8 ((uint8_t)  0xaaULL);
                uint128_t u16((uint16_t) 0xaaaaULL);
                uint128_t u32((uint32_t) 0xaaaaaaaaULL);
                uint128_t u64((uint64_t) 0xaaaaaaaaaaaaaaaa);

                const uint128_t val(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f0ULL);

                REQUIRE((t   ^  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f1ULL));
                REQUIRE((f   ^  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f0ULL));
                REQUIRE((u8  ^  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f05aULL));
                REQUIRE((u16 ^  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f05a5aULL));
                REQUIRE((u32 ^  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f05a5a5a5aULL));
                REQUIRE((u64 ^  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0x5a5a5a5a5a5a5a5aULL));

                REQUIRE((t   ^= val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f1ULL));
                REQUIRE((f   ^= val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f0ULL));
                REQUIRE((u8  ^= val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f05aULL));
                REQUIRE((u16 ^= val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f05a5aULL));
                REQUIRE((u32 ^= val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f05a5a5a5aULL));
                REQUIRE((u64 ^= val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0x5a5a5a5a5a5a5a5aULL));

                REQUIRE((uint128_t() ^ val) == val);
            }

            SECTION("External")
            {
                bool     t   = true;
                bool     f   = false;
                uint8_t  u8  = 0xaaULL;
                uint16_t u16 = 0xaaaaULL;
                uint32_t u32 = 0xaaaaaaaaULL;
                uint64_t u64 = 0xaaaaaaaaaaaaaaaaULL;

                const uint128_t val(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f0ULL);

                REQUIRE((t   ^  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f1ULL));
                REQUIRE((f   ^  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f0ULL));
                REQUIRE((u8  ^  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f05aULL));
                REQUIRE((u16 ^  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f05a5aULL));
                REQUIRE((u32 ^  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f05a5a5a5aULL));
                REQUIRE((u64 ^  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0x5a5a5a5a5a5a5a5aULL));

                REQUIRE((t   ^= val) == true);
                REQUIRE((f   ^= val) == true);
                REQUIRE((u8  ^= val) == (uint8_t)  0x5aULL);
                REQUIRE((u16 ^= val) == (uint16_t) 0x5a5aULL);
                REQUIRE((u32 ^= val) == (uint32_t) 0x5a5a5a5aULL);
                REQUIRE((u64 ^= val) == (uint64_t) 0x5a5a5a5a5a5a5a5aULL);

                // zero
                REQUIRE((uint128_t() ^ val) == val);
            }
        }

        SECTION("Invert operator")
        {
            REQUIRE(~uint128_t(0x0000000000000000ULL, 0x0000000000000000ULL) == uint128_t(0xffffffffffffffffULL, 0xffffffffffffffffULL));
            REQUIRE(~uint128_t(0x0000000000000000ULL, 0xffffffffffffffffULL) == uint128_t(0xffffffffffffffffULL, 0x0000000000000000ULL));
            REQUIRE(~uint128_t(0xffffffffffffffffULL, 0xffffffffffffffffULL) == uint128_t(0x0000000000000000ULL, 0x0000000000000000ULL));
        }
    }

    SECTION("Bitshift operator")
    {
        SECTION("Leftshift operator")
        {
            SECTION("Internal")
            {
                uint128_t val(0x1);
                uint64_t exp_val = 1;
                for(uint8_t i = 0; i < 64; i++)
                    REQUIRE((val << i) == (exp_val << i));

                uint128_t zero(0);
                for(uint8_t i = 0; i < 64; i++)
                    REQUIRE((zero << i) == 0);

                for(uint8_t i = 0; i < 63; i++) // 1 is already a bit
                    REQUIRE((val  <<= 1) == (exp_val <<= 1));

                for(uint8_t i = 0; i < 63; i++)
                    REQUIRE((zero <<= 1) == 0);
            }

            SECTION("External")
            {
                bool     t   = true;
                bool     f   = false;
                uint8_t  u8  = 0xffULL;
                uint16_t u16 = 0xffffULL;
                uint32_t u32 = 0xffffffffULL;
                uint64_t u64 = 0xffffffffffffffffULL;

                const uint128_t zero(0);
                const uint128_t one(1);

                REQUIRE(t   << zero == t);
                REQUIRE(f   << zero == f);
                REQUIRE(u8  << zero == u8);
                REQUIRE(u16 << zero == u16);
                REQUIRE(u32 << zero == u32);
                REQUIRE(u64 << zero == u64);

                REQUIRE((t   <<= zero) == t);
                REQUIRE((f   <<= zero) == f);
                REQUIRE((u8  <<= zero) == u8);
                REQUIRE((u16 <<= zero) == u16);
                REQUIRE((u32 <<= zero) == u32);
                REQUIRE((u64 <<= zero) == u64);

                REQUIRE(t   << one == uint128_t(t)   << 1);
                REQUIRE(f   << one == uint128_t(f)   << 1);
                REQUIRE(u8  << one == uint128_t(u8)  << 1);
                REQUIRE(u16 << one == uint128_t(u16) << 1);
                REQUIRE(u32 << one == uint128_t(u32) << 1);
                REQUIRE(u64 << one == uint128_t(u64) << 1);

                REQUIRE((t   <<= one) == true);
                REQUIRE((f   <<= one) == false);
                REQUIRE((u8  <<= one) == (uint8_t)  0xfeULL);
                REQUIRE((u16 <<= one) == (uint16_t) 0xfffeULL);
                REQUIRE((u32 <<= one) == (uint32_t) 0xfffffffeULL);
                REQUIRE((u64 <<= one) == (uint64_t) 0xfffffffffffffffeULL);

                REQUIRE(u8  << uint128_t(7) ==  uint128_t(0x7f00ULL));
                REQUIRE(u16 << uint128_t(15) == uint128_t(0x7fff0000ULL));
                REQUIRE(u32 << uint128_t(31) == uint128_t(0x7fffffff00000000ULL));
                REQUIRE(u64 << uint128_t(63) == uint128_t(0x7fffffffffffffff, 0x0000000000000000ULL));

                REQUIRE((u8  <<= uint128_t(7) ) ==  (uint8_t)  0);
                REQUIRE((u16 <<= uint128_t(15)) == (uint16_t) 0);
                REQUIRE((u32 <<= uint128_t(31)) == (uint32_t) 0);
                REQUIRE((u64 <<= uint128_t(63)) == (uint64_t) 0);
            }
        }

        SECTION("Rightshift operator")
        {
            SECTION("Internal")
            {
                uint128_t val(0xffffffffffffffffULL);
                uint64_t exp = 0xffffffffffffffffULL;
                for(uint8_t i = 0; i < 64; i++)
                    REQUIRE((val >> i) == (exp >> i));

                uint128_t zero(0);
                for(uint8_t i = 0; i < 64; i++)
                    REQUIRE((zero >> i) == 0);

                for(uint8_t i = 0; i < 64; i++)
                    REQUIRE((val >>= 1) == (exp >>= 1));

                for(uint8_t i = 0; i < 64; i++)
                    REQUIRE((zero >>= 1) == 0);
            }

            SECTION("External")
            {
                bool     t   = true;
                bool     f   = false;
                uint8_t  u8  = 0xffULL;
                uint16_t u16 = 0xffffULL;
                uint32_t u32 = 0xffffffffULL;
                uint64_t u64 = 0xffffffffffffffffULL;

                const uint128_t zero(0);
                const uint128_t one(1);

                REQUIRE((t   >> zero) == one);
                REQUIRE((f   >> zero) == zero);
                REQUIRE((u8  >> zero) == u8);
                REQUIRE((u16 >> zero) == u16);
                REQUIRE((u32 >> zero) == u32);
                REQUIRE((u64 >> zero) == u64);

                REQUIRE((t   >>= zero) == t);
                REQUIRE((f   >>= zero) == f);
                REQUIRE((u8  >>= zero) == u8);
                REQUIRE((u16 >>= zero) == u16);
                REQUIRE((u32 >>= zero) == u32);
                REQUIRE((u64 >>= zero) == u64);

                REQUIRE((t   >> one) == uint128_t(t)   >> 1);
                REQUIRE((f   >> one) == uint128_t(f)   >> 1);
                REQUIRE((u8  >> one) == uint128_t(u8)  >> 1);
                REQUIRE((u16 >> one) == uint128_t(u16) >> 1);
                REQUIRE((u32 >> one) == uint128_t(u32) >> 1);
                REQUIRE((u64 >> one) == uint128_t(u64) >> 1);

                REQUIRE((t   >>= one) == false);
                REQUIRE((f   >>= one) == false);
                REQUIRE((u8  >>= one) == (uint8_t)  0x7fULL);
                REQUIRE((u16 >>= one) == (uint16_t) 0x7fffULL);
                REQUIRE((u32 >>= one) == (uint32_t) 0x7fffffffULL);
                REQUIRE((u64 >>= one) == (uint64_t) 0x7fffffffffffffffULL);

                REQUIRE((u8  >> uint128_t(7)) ==  zero);
                REQUIRE((u16 >> uint128_t(15)) == zero);
                REQUIRE((u32 >> uint128_t(31)) == zero);
                REQUIRE((u64 >> uint128_t(63)) == zero);

                REQUIRE((u8  >>= uint128_t(7)) ==  (uint8_t)  0);
                REQUIRE((u16 >>= uint128_t(15)) == (uint16_t) 0);
                REQUIRE((u32 >>= uint128_t(31)) == (uint32_t) 0);
                REQUIRE((u64 >>= uint128_t(63)) == (uint64_t) 0);
            }
        }
    }

    SECTION("Logical operators")
    {
        SECTION("Equal operator")
        {
            REQUIRE( (uint128_t(0xdeadbeefULL) == uint128_t(0xdeadbeefULL)) == true);
            REQUIRE(!(uint128_t(0xdeadbeefULL) == uint128_t(0xfee1baadULL)) == true);

            const bool     t   = true;
            const bool     f   = false;
            const uint8_t  u8  = 0xaaULL;
            const uint16_t u16 = 0xaaaaULL;
            const uint32_t u32 = 0xaaaaaaaaULL;
            const uint64_t u64 = 0xaaaaaaaaaaaaaaaaULL;

            REQUIRE((t)   == uint128_t(t));
            REQUIRE((f)   == uint128_t(f));
            REQUIRE((u8)  == uint128_t(u8));
            REQUIRE((u16) == uint128_t(u16));
            REQUIRE((u32) == uint128_t(u32));
            REQUIRE((u64) == uint128_t(u64));
        }

        SECTION("Not Equals operator")
        {
            REQUIRE(!(uint128_t(0xdeadbeefULL) != uint128_t(0xdeadbeefULL)) == true);
            REQUIRE( (uint128_t(0xdeadbeefULL) != uint128_t(0xfee1baadULL)) == true);

            const bool     t   = true;
            const bool     f   = false;
            const uint8_t  u8  = 0xaaULL;
            const uint16_t u16 = 0xaaaaULL;
            const uint32_t u32 = 0xaaaaaaaaULL;
            const uint64_t u64 = 0xaaaaaaaaaaaaaaaaULL;

            REQUIRE((t   != uint128_t(f)) ==   true);
            REQUIRE((f   != uint128_t(t)) ==   true);
            REQUIRE((u8  != uint128_t(u64)) == true);
            REQUIRE((u16 != uint128_t(u32)) == true);
            REQUIRE((u32 != uint128_t(u16)) == true);
            REQUIRE((u64 != uint128_t(u8)) ==  true);

            REQUIRE((t   != uint128_t(t)) ==   false);
            REQUIRE((f   != uint128_t(f)) ==   false);
            REQUIRE((u8  != uint128_t(u8)) ==  false);
            REQUIRE((u16 != uint128_t(u16)) == false);
            REQUIRE((u32 != uint128_t(u32)) == false);
            REQUIRE((u64 != uint128_t(u64)) == false);
        }

        SECTION("And operator")
        {
            const uint128_t A(0xffffffff);
            const uint128_t B(0x00000000);

            REQUIRE((A && A) == true);
            REQUIRE((A && B) == false);
        }

        SECTION("Or operator")
        {
            const uint128_t A(0xffffffff);
            const uint128_t B(0x00000000);

            REQUIRE((A || A) == true);
            REQUIRE((A || B) == true);
        }

        SECTION("Not operator")
        {
            REQUIRE(static_cast<int>(!uint128_t(0xffffffff)) ==  0);
        }

        SECTION("Greater than operator")
        {
            const uint128_t big  (0xffffffffffffffffULL, 0xffffffffffffffffULL);
            const uint128_t small(0x0000000000000000ULL, 0x0000000000000000ULL);

            REQUIRE((small > small) ==   false);
            REQUIRE((small > big)   ==   false);

            REQUIRE((big > small)   ==   true);
            REQUIRE((big > big)     ==   false);
        }

        SECTION("Greater Or Equal operator")
        {
            const uint128_t big  (0xffffffffffffffffULL, 0xffffffffffffffffULL);
            const uint128_t small(0x0000000000000000ULL, 0x0000000000000000ULL);

            REQUIRE((small >= small) ==  true);
            REQUIRE((small >= big)   ==  false);

            REQUIRE((big >= small)   ==  true);
            REQUIRE((big >= big)     ==  true);
        }

        SECTION("Less than operator")
        {
            const uint128_t big  (0xffffffffffffffffULL, 0xffffffffffffffffULL);
            const uint128_t small(0x0000000000000000ULL, 0x0000000000000000ULL);

            REQUIRE((small < small) == false);
            REQUIRE((small < big)   == true);

            REQUIRE((big < small)   == false);
            REQUIRE((big < big)     == false);
        }

        SECTION("Less than or Equal operator")
        {
            const uint128_t big  (0xffffffffffffffffULL, 0xffffffffffffffffULL);
            const uint128_t small(0x0000000000000000ULL, 0x0000000000000000ULL);

            REQUIRE((small <= small) == true);
            REQUIRE((small <= big)   == true);

            REQUIRE((big <= small)   == false);
            REQUIRE((big <= big)     == true);
        }
    }

    SECTION("Arithmetic operators")
    {
        SECTION("Add operator")
        {
            SECTION("Internal")
            {
                uint128_t low (0, 1);
                uint128_t high(1, 0);

                REQUIRE((low  + low)  ==  2);
                REQUIRE((low  + high) == uint128_t(1, 1));
                REQUIRE((high + high) == uint128_t(2, 0));

                REQUIRE((low  += low)  ==  2);
                REQUIRE((low  += high) == uint128_t(1, 2));
                REQUIRE((high += low)  ==  uint128_t(2, 2));
            }

            SECTION("External")
            {
                bool     t   = true;
                bool     f   = false;
                uint8_t  u8  = 0xaaULL;
                uint16_t u16 = 0xaaaaULL;
                uint32_t u32 = 0xaaaaaaaaULL;
                uint64_t u64 = 0xaaaaaaaaaaaaaaaaULL;

                const uint128_t val(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f0ULL);

                REQUIRE((t   +  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f1ULL));
                REQUIRE((f   +  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f0ULL));
                REQUIRE((u8  +  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f19aULL));
                REQUIRE((u16 +  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f19b9aULL));
                REQUIRE((u32 +  val) == uint128_t(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f19b9b9b9aULL));
                REQUIRE((u64 +  val) == uint128_t(0xf0f0f0f0f0f0f0f1ULL, 0x9b9b9b9b9b9b9b9aULL));

                REQUIRE((t   += val) == true);
                REQUIRE((f   += val) == true);
                REQUIRE((u8  += val) == (uint8_t)  0x9aULL);
                REQUIRE((u16 += val) == (uint16_t) 0x9b9aULL);
                REQUIRE((u32 += val) == (uint32_t) 0x9b9b9b9aULL);
                REQUIRE((u64 += val) == (uint64_t) 0x9b9b9b9b9b9b9b9aULL);
            }
        }

        SECTION("Subtract operator")
        {
            SECTION("Internal")
            {
                uint128_t big  (0xffffffffffffffffULL, 0xffffffffffffffffULL);
                uint128_t small(1);

                REQUIRE((small - small) == 0);
                REQUIRE((small - big)   == uint128_t(0, 2));
                REQUIRE((big   - small) == uint128_t(0xffffffffffffffffULL, 0xfffffffffffffffeULL));
                REQUIRE((big   - big)   == 0);
            }

            SECTION("External")
            {
                bool     t   = true;
                bool     f   = false;
                uint8_t  u8  = 0xaaULL;
                uint16_t u16 = 0xaaaaULL;
                uint32_t u32 = 0xaaaaaaaaULL;
                uint64_t u64 = 0xaaaaaaaaaaaaaaaaULL;

                const uint128_t val(0xf0f0f0f0f0f0f0f0ULL, 0xf0f0f0f0f0f0f0f0ULL);

                REQUIRE((t   -  val) == uint128_t(0x0f0f0f0f0f0f0f0fULL, 0x0f0f0f0f0f0f0f11ULL));
                REQUIRE((f   -  val) == uint128_t(0x0f0f0f0f0f0f0f0fULL, 0x0f0f0f0f0f0f0f10ULL));
                REQUIRE((u8  -  val) == uint128_t(0x0f0f0f0f0f0f0f0fULL, 0x0f0f0f0f0f0f0fbaULL));
                REQUIRE((u16 -  val) == uint128_t(0x0f0f0f0f0f0f0f0fULL, 0x0f0f0f0f0f0fb9baULL));
                REQUIRE((u32 -  val) == uint128_t(0x0f0f0f0f0f0f0f0fULL, 0x0f0f0f0fb9b9b9baULL));
                REQUIRE((u64 -  val) == uint128_t(0x0f0f0f0f0f0f0f0fULL, 0xb9b9b9b9b9b9b9baULL));

                REQUIRE((t   -= val) == true);
                REQUIRE((f   -= val) == true);
                REQUIRE((u8  -= val) == (uint8_t)  0xbaULL);
                REQUIRE((u16 -= val) == (uint16_t) 0xb9baULL);
                REQUIRE((u32 -= val) == (uint32_t) 0xb9b9b9baULL);
                REQUIRE((u64 -= val) == (uint64_t) 0xb9b9b9b9b9b9b9baULL);
            }
        }

        SECTION("Multiply operator")
        {
            SECTION("Internal")
            {
                uint128_t val(0xfedbca9876543210ULL);

                REQUIRE((val * val) == uint128_t(0xfdb8e2bacbfe7cefULL, 0x010e6cd7a44a4100ULL));

                const uint128_t zero = 0;
                REQUIRE((val  * zero) == zero);
                REQUIRE((zero * val) ==  zero);

                const uint128_t one = 1;
                REQUIRE((val * one) == val);
                REQUIRE((one * val) == val);
            }

            SECTION("External")
            {
                bool     t   = true;
                bool     f   = false;
                uint8_t  u8  = 0xaaULL;
                uint16_t u16 = 0xaaaaULL;
                uint32_t u32 = 0xaaaaaaaaULL;
                uint64_t u64 = 0xaaaaaaaaaaaaaaaaULL;

                const uint128_t val(0xf0f0f0f0f0f0f0f0, 0xf0f0f0f0f0f0f0f0ULL);

                REQUIRE((t   *  val) == val);
                REQUIRE((f   *  val) == 0);
                REQUIRE((u8  *  val) == uint128_t(0xffffffffffffffff, 0xffffffffffffff60ULL));
                REQUIRE((u16 *  val) == uint128_t(0xffffffffffffffff, 0xffffffffffff5f60ULL));
                REQUIRE((u32 *  val) == uint128_t(0xffffffffffffffff, 0xffffffff5f5f5f60ULL));
                REQUIRE((u64 *  val) == uint128_t(0xffffffffffffffff, 0x5f5f5f5f5f5f5f60ULL));

                REQUIRE((t   *= val) == true);
                REQUIRE((f   *= val) == false);
                REQUIRE((u8  *= val) == (uint8_t)                0x60ULL);
                REQUIRE((u16 *= val) == (uint16_t)             0x5f60ULL);
                REQUIRE((u32 *= val) == (uint32_t)         0x5f5f5f60ULL);
                REQUIRE((u64 *= val) == (uint64_t) 0x5f5f5f5f5f5f5f60ULL);
            }
        }

        SECTION("Divide operator")
        {
            SECTION("Internal")
            {
                const uint128_t big_val  (0xfedbca9876543210ULL);
                const uint128_t small_val(0xffffULL);
                const uint128_t res_val  (0xfedcc9753fc9ULL);

                REQUIRE((small_val / small_val) == 1);
                REQUIRE((small_val / big_val) ==   0);

                REQUIRE((big_val   / big_val) ==   1);

                REQUIRE_THROWS_AS((uint128_t(1) / uint128_t(0)), std::domain_error);
            }

            SECTION("External")
            {
                bool     t   = true;
                bool     f   = false;
                uint8_t  u8  = 0xaaULL;
                uint16_t u16 = 0xaaaaULL;
                uint32_t u32 = 0xaaaaaaaaULL;
                uint64_t u64 = 0xaaaaaaaaaaaaaaaaULL;

                const uint128_t val(0x7bULL);

                REQUIRE((t   /  val) == false);
                REQUIRE((f   /  val) == false);
                REQUIRE((u8  /  val) == uint128_t(0x1ULL));
                REQUIRE((u16 /  val) == uint128_t(0x163ULL));
                REQUIRE((u32 /  val) == uint128_t(0x163356bULL));
                REQUIRE((u64 /  val) == uint128_t(0x163356b88ac0de0ULL));

                REQUIRE((t   /= val) == false);
                REQUIRE((f   /= val) == false);
                REQUIRE((u8  /= val) == (uint8_t)  0x1ULL);
                REQUIRE((u16 /= val) == (uint16_t) 0x163ULL);
                REQUIRE((u32 /= val) == (uint32_t) 0x163356bULL);
                REQUIRE((u64 /= val) == (uint64_t) 0x163356b88ac0de0ULL);
            }
        }

        SECTION("Mod operator")
        {
            SECTION("Internal")
            {
                // has remainder
                const uint128_t val    (0xffffffffffffffffULL, 0xffffffffffffffffULL);
                const uint128_t val_mod(0xfedcba9876543210ULL);

                REQUIRE((val % val_mod) == uint128_t(0x7f598f328cc265bfULL));

                // no remainder
                const uint128_t val_0  (0xfedcba9876543210, 0);
                REQUIRE((val_0 % val_mod) == 0);

                // mod 0
                REQUIRE_THROWS_AS((uint128_t(1) % uint128_t(0)), std::domain_error);
            }

            SECTION("External")
            {
                bool     t   = true;
                bool     f   = false;
                uint8_t  u8  = 0xaaULL;
                uint16_t u16 = 0xaaaaULL;
                uint32_t u32 = 0xaaaaaaaaULL;
                uint64_t u64 = 0xaaaaaaaaaaaaaaaaULL;

                const uint128_t val(0xd03ULL); // prime

                REQUIRE((t   %  val) == true);
                REQUIRE((f   %  val) == false);
                REQUIRE((u8  %  val) == uint128_t(0xaaULL));
                REQUIRE((u16 %  val) == uint128_t(0x183ULL));
                REQUIRE((u32 %  val) == uint128_t(0x249ULL));
                REQUIRE((u64 %  val) == uint128_t(0xc7fULL));

                REQUIRE((t   %= val) == true);
                REQUIRE((f   %= val) == false);
                REQUIRE((u8  %= val) == (uint8_t)  0xaaULL);
                REQUIRE((u16 %= val) == (uint16_t) 0x183ULL);
                REQUIRE((u32 %= val) == (uint32_t) 0x249ULL);
                REQUIRE((u64 %= val) == (uint64_t) 0xc7fULL);
            }
        }
    }

    SECTION("Unary operators")
    {
        SECTION("Plus operator")
        {
            const uint128_t value(0x12345ULL);
            REQUIRE(+value == value);
        }

        SECTION("Minus operator")
        {
            const uint128_t val(1);
            const uint128_t neg = -val;
            REQUIRE(-val == neg);
            REQUIRE(-neg == val);
            REQUIRE(neg == uint128_t(0xffffffffffffffffULL, 0xffffffffffffffffULL));
        }
    }
}
