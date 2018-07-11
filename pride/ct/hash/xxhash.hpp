
#pragma once
#include <array>
#include <cstdint>
#include <limits>

namespace pride::ct::hash
{
    namespace detail::xxhash
    {
        template<typename T> struct constant;

        template<>
        struct constant<hash32_t>
        {
            using hash_type = std::uint32_t;
            using long_type = std::uint64_t;
            static constexpr uint32_t seed = 20180101UL;
            static constexpr std::array<hash_type, 5> primes {{
                2654435761U, 2246822519U, 3266489917U, 668265263U, 374761393U
            }};
        };

        template<>
        struct constant<hash64_t>
        {
            using hash_type = std::uint64_t;
            using long_type = std::uintmax_t;
            static constexpr uint64_t seed = 201801011200ULL;
            static constexpr std::array<hash_type, 5> primes {{
                11400714785074694791ULL, 14029467366897019727ULL, 1609587929392839161ULL, 9650029242287828579ULL, 2870177450012600261ULL
            }};
        };

        template<typename T>
        constexpr T rotl(T n, size_t shift)
        {
            static_assert(std::numeric_limits<T>::is_integer, "Type can only be integers");
            return ((n << shift) | (n >> (std::numeric_limits<T>::digits - shift)));
        }

        template<typename Hash, typename Char>
        constexpr Hash read(const Char* p)
        {
            return static_cast<Hash>(*p);
        }
        template <typename Char>
		constexpr std::uint8_t read(const Char* p)
		{
			return std::uint8_t(p[0]);
		}
        template <typename Char>
		constexpr std::uint16_t read(const Char* p)
		{
			return ((static_cast<std::uint16_t>(std::uint8_t(p[0])) << 0)
				| (static_cast<std::uint16_t>(std::uint8_t(p[1])) << 8));
		}
        template <typename Char>
		constexpr std::uint32_t read(const Char* p)
		{
			return ((static_cast<std::uint32_t>(std::uint8_t(p[0])) << 0)
				| (static_cast<std::uint32_t>(std::uint8_t(p[1])) << 8)
				| (static_cast<std::uint32_t>(std::uint8_t(p[2])) << 16)
				| (static_cast<std::uint32_t>(std::uint8_t(p[3])) << 24));
		}
        template <typename Char>
		constexpr std::uint64_t read(const Char* p)
		{
			return ((static_cast<std::uint64_t>(std::uint8_t(p[0])) << 0)
				| (static_cast<std::uint64_t>(std::uint8_t(p[1])) << 8)
				| (static_cast<std::uint64_t>(std::uint8_t(p[2])) << 16)
				| (static_cast<std::uint64_t>(std::uint8_t(p[3])) << 24)
				| (static_cast<std::uint64_t>(std::uint8_t(p[4])) << 32)
				| (static_cast<std::uint64_t>(std::uint8_t(p[5])) << 40)
				| (static_cast<std::uint64_t>(std::uint8_t(p[6])) << 48)
				| (static_cast<std::uint64_t>(std::uint8_t(p[7])) << 56));
		}

        // ─────────────────────────────────────────────── PROTOTYPING ─────

        template<typename Hash>
        constexpr Hash round(Hash acc, Hash input);

        template<typename Hash>
        constexpr Hash convergence(Hash acc0, Hash acc1, Hash acc2, Hash acc3);

        template<typename Hash>
        constexpr Hash final_mix(Hash acc);

        // ─────────────────────────────────────────────────── HASH 32 ─────

        template<>
        constexpr hash32_t round(hash32_t acc, hash32_t input)
        {
            using hash_type = constant<hash32_t>::hash_type;
            using long_type = constant<hash32_t>::long_type;

            return static_cast<hash_type>(
                rotl<hash_type>(
                    static_cast<hash_type>(
                        static_cast<long_type>(
                            static_cast<long_type>(acc)
                            + (static_cast<long_type>(input)
                            * static_cast<long_type>(constant<hash_type>::primes[1]))
                        )
                    ), 13
                )
                * static_cast<long_type>(constant<hash32_t>::primes[0])
            );
        }

        template<>
        constexpr hash32_t convergence(hash32_t acc0, hash32_t acc1, hash32_t acc2, hash32_t acc3)
        {
            using hash_type = constant<hash32_t>::hash_type;
            using long_type = constant<hash32_t>::long_type;

            return static_cast<hash_type>(
                static_cast<long_type>(rotl(acc0, 1))
                + static_cast<long_type>(rotl(acc1, 7))
                + static_cast<long_type>(rotl(acc2, 12))
                + static_cast<long_type>(rotl(acc3, 18))
            );
        }

        template<>
        constexpr hash32_t final_mix(hash32_t acc)
        {
            using hash_type = constant<hash32_t>::hash_type;
            using long_type = constant<hash32_t>::long_type;

            acc = static_cast<hash_type>((acc ^ (acc >> 15)) * static_cast<long_type>(constant<hash_type>::primes[1]));
            acc = static_cast<hash_type>((acc ^ (acc >> 13)) * static_cast<long_type>(constant<hash_type>::primes[2]));
            acc = acc ^ (acc >> 16);
            return acc;
        }

        // ─────────────────────────────────────────────────── HASH 64 ─────

        template<>
        constexpr hash64_t round(hash64_t acc, hash64_t input)
        {
            using hash_type = constant<hash64_t>::hash_type;
            using long_type = constant<hash64_t>::long_type;

            acc = static_cast<hash_type>(acc + (static_cast<long_type>(input) * static_cast<long_type>(constant<hash_type>::primes[1])));
            acc = rotl(acc, 31);
            return static_cast<hash_type>(acc * static_cast<long_type>(constant<hash_type>::primes[0]));
        }

        inline constexpr hash64_t mearge_accumulator(hash64_t acc, hash64_t acc_n)
        {
            using hash_type = constant<hash64_t>::hash_type;
            using long_type = constant<hash64_t>::long_type;

            return static_cast<hash_type>(
                ((acc ^ round(static_cast<hash_type>(0), acc_n))
                * static_cast<long_type>(constant<hash_type>::primes[0]))
                + static_cast<long_type>(constant<hash_type>::primes[3])
            );
        }

        template<>
        constexpr hash64_t convergence(hash64_t acc0, hash64_t acc1, hash64_t acc2, hash64_t acc3)
        {
            using hash_type = constant<hash64_t>::hash_type;
            using long_type = constant<hash64_t>::long_type;

            hash_type acc = static_cast<hash_type>(
                static_cast<long_type>(rotl(acc0, 1))
                + static_cast<long_type>(rotl(acc1, 7))
                + static_cast<long_type>(rotl(acc2, 12))
                + static_cast<long_type>(rotl(acc3, 18))
            );

            acc = mearge_accumulator(acc, acc0);
            acc = mearge_accumulator(acc, acc1);
            acc = mearge_accumulator(acc, acc2);
            acc = mearge_accumulator(acc, acc3);
            return acc;
        }

        template<>
        constexpr hash64_t final_mix(hash64_t acc)
        {
            using hash_type = constant<hash64_t>::hash_type;
            using long_type = constant<hash64_t>::long_type;

            acc = static_cast<hash_type>((acc ^ (acc >> 33)) * static_cast<long_type>(constant<hash_type>::primes[1]));
            acc = static_cast<hash_type>((acc ^ (acc >> 29)) * static_cast<long_type>(constant<hash_type>::primes[2]));
            acc = (acc ^ (acc >> 32));
            return acc;
        }

        // ─────────────────────────────────────────────────── COMPUTE ─────

        template<typename Hash, typename Char>
        constexpr Hash compute(const Char* key, size_t len, Hash seed);

        template<typename Char>
        constexpr hash32_t compute(const Char* key, size_t len, hash32_t seed)
        {
            using hash_type = constant<hash32_t>::hash_type;
            using long_type = constant<hash32_t>::long_type;

            hash_type acc = 0;
            if (key == nullptr)
            {
                len = 0;

                // step 1.
                acc = static_cast<hash_type>(seed + static_cast<long_type>(constant<hash_type>::primes[4]));

                // step 4.
                acc = static_cast<hash_type>(static_cast<long_type>(acc) + static_cast<long_type>(len));

                // step 6.
                acc = final_mix(acc);

                // step 7.
                return acc;
            }

            const Char* src{ key };
            const Char* end{ src + len };

            // step 1.
            if (len >= 16)
            {
                const Char* limit{ end - 16 };
                hash_type acc0 = static_cast<hash_type>(seed + static_cast<long_type>(constant<hash_type>::primes[0]) + static_cast<long_type>(constant<hash_type>::primes[1]));
                hash_type acc1 = static_cast<hash_type>(seed + static_cast<long_type>(constant<hash_type>::primes[1]));
                hash_type acc2 = seed + 0;
                hash_type acc3 = seed - constant<hash_type>::primes[0];

                // step 2.
                do
                {
                    acc0 = round(acc0, read<std::uint32_t>(src));
                    src += sizeof(std::uint32_t);
                    acc1 = round(acc1, read<std::uint32_t>(src));
                    src += sizeof(std::uint32_t);
                    acc2 = round(acc2, read<std::uint32_t>(src));
                    src += sizeof(std::uint32_t);
                    acc3 = round(acc3, read<std::uint32_t>(src));
                    src += sizeof(std::uint32_t);
                } while (src <= limit);

                // step 3.
                acc = convergence(acc0, acc1, acc2, acc3);
            }
            else
            {
                acc = static_cast<hash_type>(seed + static_cast<long_type>(constant<hash_type>::primes[4]));
            }

            // step 4.
            acc = static_cast<hash_type>(static_cast<long_type>(acc) + static_cast<long_type>(len));

            // step 5.
            if (len >= 4) {
                const Char* limit{ end - 4 };
                while (src <= limit) {
                    acc = static_cast<hash_type>(
                        rotl<hash_type>(
                            static_cast<hash_type>(static_cast<long_type>(acc)
                            + static_cast<long_type>((read<std::uint32_t>(src))
                            * static_cast<long_type>(constant<hash_type>::primes[2]))), 17
                        )
                        * static_cast<long_type>(constant<hash_type>::primes[3])
                    );
                    src += sizeof(std::uint32_t);
                }
            }

            if (key != nullptr)
            {
                while (src < end)
                {
                    acc = static_cast<hash_type>(
                        rotl<hash_type>(
                            static_cast<long_type>(acc)
                            + static_cast<long_type>(read<std::uint8_t>(src))
                            * static_cast<long_type>(constant<hash_type>::primes[4]), 11
                        )
                        * static_cast<long_type>(constant<hash_type>::primes[0])
                    );
                    ++src;
                }
            }

            // step 6.
            acc = final_mix(acc);

            // step 7.
            return acc;
        }

        template<typename Char>
        constexpr hash64_t compute(const Char* key, size_t len, hash64_t seed)
        {
            using hash_type = constant<hash64_t>::hash_type;
            using long_type = constant<hash64_t>::long_type;

            hash_type acc = 0;
            if (key == nullptr)
            {
                len = 0;

                // step 1.
                acc = static_cast<hash_type>(seed + static_cast<long_type>(constant<hash_type>::primes[4]));

                // step 4.
                acc += len;

                // step 6.
                acc = final_mix(acc);

                // step 7.
                return acc;
            }
            const Char* src{ key };
            const Char* end{ src + len };

            // step 1.
            if (len >= 32)
            {
                const Char* limit{ end - 32 };
                hash_type acc0 = static_cast<hash_type>(seed + static_cast<long_type>(constant<hash_type>::primes[0]) + static_cast<long_type>(constant<hash_type>::primes[1]));
                hash_type acc1 = static_cast<hash_type>(seed + static_cast<long_type>(constant<hash_type>::primes[1]));
                hash_type acc2 = seed + 0;
                hash_type acc3 = seed - constant<hash_type>::primes[0];

                // step 2.
                do
                {
                    acc0 = round(acc0, read<std::uint64_t>(src));
                    src += sizeof(std::uint64_t);
                    acc1 = round(acc1, read<std::uint64_t>(src));
                    src += sizeof(std::uint64_t);
                    acc2 = round(acc2, read<std::uint64_t>(src));
                    src += sizeof(std::uint64_t);
                    acc3 = round(acc3, read<std::uint64_t>(src));
                    src += sizeof(std::uint64_t);
                } while (src <= limit);

                // step 3.
                acc = convergence(acc0, acc1, acc2, acc3);
            }
            else
            {
                acc = static_cast<hash_type>(seed + static_cast<long_type>(constant<hash_type>::primes[4]));
            }

            // step 4.
            acc += len;

            // step 5.
            if (len >= 8)
            {
                const Char* limit{ end - 8 };
                while (src <= limit)
                {
                    acc = static_cast<hash_type>(
                        rotl<std::uint64_t>(
                            acc ^ round(static_cast<hash_type>(0), read<std::uint64_t>(src)), 27
                        )
                        * static_cast<long_type>(
                            constant<hash_type>::primes[0]) + static_cast<long_type>(constant<hash_type>::primes[3]
                        )
                    );
                    src += sizeof(std::uint64_t);
                }
            }

            if (len >= 4)
            {
                const Char* limit{ end - 4 };
                while (src <= limit)
                {
                    acc = static_cast<hash_type>(
                        rotl<hash_type>(
                            acc ^ static_cast<hash_type>((read<std::uint32_t>(src) * static_cast<long_type>(constant<hash_type>::primes[0]))), 23
                        )
                        * static_cast<long_type>(constant<hash_type>::primes[1])
                        + static_cast<long_type>(constant<hash_type>::primes[2])
                    );
                    src += sizeof(std::uint32_t);
                }
            }

            if (key != nullptr)
            {
                while (src < end)
                {
                    acc = static_cast<hash_type>(
                        rotl<hash_type>(
                            acc ^ (read<std::uint8_t>(src) * static_cast<long_type>(constant<hash_type>::primes[4])), 11
                        )
                        * static_cast<long_type>(constant<hash_type>::primes[0])
                    );
                    ++src;
                }
            }

            // step 6.
            acc = final_mix(acc);

            // step 7.
            return acc;
        }

        // ─────────────────────────────────────────────────────────────────

        template<typename Hash, typename Char>
        constexpr Hash xxhash(const Char* data, size_t len, Hash seed = constant<Hash>::seed)
        {
            static_assert(std::is_same<Char, char>() || std::is_same<Char, wchar_t>(), "Input type is not supported");
            static_assert(std::is_same<Hash, hash64_t>() || std::is_same<Hash, hash32_t>(), "Hash type is not supported");
            return compute(data, len, seed);
        }
    } // namespace detail

    template<typename Char, size_t N>
    constexpr hash_t xxhash(const Char(&input)[N])
    {
        return detail::xxhash::xxhash<hash_t>(input, N - 1);
    }

    template<typename Char>
    constexpr hash_t xxhash(const Char* input, const size_t len)
    {
        return detail::xxhash::xxhash<hash_t>(input, len);
    }

    template<typename Char, size_t N>
    constexpr hash32_t xxhash32(const Char(&input)[N])
    {
        return detail::xxhash::xxhash<hash32_t>(input, N - 1);
    }

    template<typename Char>
    constexpr hash32_t xxhash32(const Char* input, const size_t len)
    {
        return detail::xxhash::xxhash<hash32_t>(input, len);
    }

    template<typename Char, size_t N>
    constexpr hash64_t xxhash64(const Char(&input)[N])
    {
        return detail::xxhash::xxhash<hash64_t>(input, N - 1);
    }

    template<typename Char>
    constexpr hash64_t xxhash64(const Char* input, const size_t len)
    {
        return detail::xxhash::xxhash<hash64_t>(input, len);
    }
}

constexpr pride::hash_t operator "" _xxhash(const char* ptr, const size_t len)
{
    return pride::ct::hash::xxhash(ptr, len);
}

constexpr pride::hash_t operator "" _xxhash(const wchar_t* ptr, const size_t len)
{
    return pride::ct::hash::xxhash(ptr, len);
}

constexpr pride::hash32_t operator "" _xxhash32(const char* ptr, const size_t len)
{
    return pride::ct::hash::xxhash(ptr, len);
}

constexpr pride::hash32_t operator "" _xxhash32(const wchar_t* ptr, const size_t len)
{
    return pride::ct::hash::xxhash(ptr, len);
}

constexpr pride::hash64_t operator "" _xxhash64(const char* ptr, const size_t len)
{
    return pride::ct::hash::xxhash(ptr, len);
}

constexpr pride::hash64_t operator "" _xxhash64(const wchar_t* ptr, const size_t len)
{
    return pride::ct::hash::xxhash(ptr, len);
}

#if defined(PRI_USE_CT_TEST)
    static_assert(pride::ct::hash::xxhash("This should") != pride::ct::hash::xxhash("not match"));
    static_assert(pride::ct::hash::xxhash("This should") != "not match"_xxhash);
    static_assert(pride::ct::hash::xxhash("This should match") == "This should match"_xxhash);
#endif
