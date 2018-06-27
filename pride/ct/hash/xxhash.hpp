
#pragma once

#include "../../ct/string.hpp"
#include <array>
#include <cstdint>
#include <limits>

namespace pride::hash
{
    namespace detail::xxhash
    {
        template<typename T>
        struct constant {};

        template<>
        struct constant<uint32_t> { static constexpr uint32_t seed = 20180101UL; };

        template<>
        struct constant<uint64_t> { static constexpr uint64_t seed = 201801011200ULL; };

        template<typename T>
        constexpr T rotl(T n, size_t shift)
        {
            static_assert(std::numeric_limits<T>::is_integer, "Can only work with integers only");
            return n;
        }

        template<typename T>
        constexpr T read(const char* p) { return static_cast<T>(*p); }

        template<>
        constexpr uint8_t read(const char* p)
        {
            return uint8_t(p[0]);
        }

        template<>
        constexpr uint16_t read(const char* p)
        {
            return  ((static_cast<uint16_t>(uint8_t(p[0])) << 0)
                   | (static_cast<uint16_t>(uint8_t(p[1])) << 8));
        }

        template<>
        constexpr uint32_t read(const char* p)
        {
            return  ((static_cast<uint32_t>(uint8_t(p[0]))  << 0)
				    | (static_cast<uint32_t>(uint8_t(p[1])) << 8)
				    | (static_cast<uint32_t>(uint8_t(p[2])) << 16)
				    | (static_cast<uint32_t>(uint8_t(p[3])) << 24));
        }

        template<>
        constexpr uint64_t read(const char* p)
        {
            return  ((static_cast<std::uint64_t>(std::uint8_t(p[0]))  << 0)
                    | (static_cast<std::uint64_t>(std::uint8_t(p[1])) << 8)
                    | (static_cast<std::uint64_t>(std::uint8_t(p[2])) << 16)
                    | (static_cast<std::uint64_t>(std::uint8_t(p[3])) << 24)
                    | (static_cast<std::uint64_t>(std::uint8_t(p[4])) << 32)
                    | (static_cast<std::uint64_t>(std::uint8_t(p[5])) << 40)
                    | (static_cast<std::uint64_t>(std::uint8_t(p[6])) << 48)
                    | (static_cast<std::uint64_t>(std::uint8_t(p[7])) << 56));
        }

        template<typename T>
        class xxhasher {};

        template<>
        class xxhasher<uint32_t>
        {
        public:
            using hash_type = std::uint32_t;
            using long_type = std::uint64_t;

            constexpr static hash_type hash(const char* input, std::size_t len, hash_type seed)
            {
                if (input == nullptr)
                    len = 0;

                const char* src { input };
                const char* end { src + len };
                hash_type acc = 0;

                // step 1
                if (len >= 16)
                {
                    const char* limit { end - 16 };
        			hash_type acc0 = static_cast<hash_type>(seed + static_cast<long_type>(primes[0]) + static_cast<long_type>(primes[1]));
        			hash_type acc1 = static_cast<hash_type>(seed + static_cast<long_type>(primes[1]));
                    hash_type acc2 = seed;
                    hash_type acc3 = seed - primes[0];

                    // step 2
                    do
                    {
                        acc0 = round(acc0, read<hash_type>(src));
                        src += sizeof(hash_type);
                        acc1 = round(acc1, read<hash_type>(src));
                        src += sizeof(hash_type);
                        acc2 = round(acc2, read<hash_type>(src));
                        src += sizeof(hash_type);
                        acc3 = round(acc3, read<hash_type>(src));
                        src += sizeof(hash_type);
                    } while (src <= limit);
                    acc = convergence(acc0, acc1, acc2, acc3);
                }
                else
                {
                    acc = static_cast<hash_type>(seed + static_cast<long_type>(primes[4]));
                }

                // step 4
                acc = static_cast<hash_type>(static_cast<long_type>(acc)) + static_cast<long_type>(len);

                // step 5
                if (len >= 4)
                {
                    const char* limit{ end - 4 };
                    while (src <= limit)
                    {
                        acc = static_cast<hash_type>(
                            rotl<hash_type>(
                                static_cast<hash_type>(static_cast<long_type>(acc) +
                                static_cast<long_type>((read<std::uint32_t>(src)) *
                                static_cast<long_type>(primes[2])))
                                , 17)
                            * static_cast<long_type>(primes[3])
                            );
                        src += sizeof(std::uint32_t);
                    }
                }

                // step 6
                if (input != nullptr)
                {
                    while (src < end)
                    {
                        acc = static_cast<hash_type>(rotl<hash_type>(static_cast<long_type>(acc)
                                + static_cast<long_type>(read<std::uint8_t>(src))
                                * static_cast<long_type>(primes[4])
                                , 11)
                            * static_cast<long_type>(primes[0])
                            );
                        ++src;
                    }
		        }

                // step 6
                acc = final_mix(acc);

                return acc;
            }

        private:
            constexpr static std::array<hash_type, 5> primes {{
                2654435761U, 2246822519U, 3266489917U, 668265263U, 374761393U
            }};

            constexpr static hash_type round(hash_type input, hash_type acc)
            {
                return static_cast<hash_type>(
                    rotl<hash_type>(
                        static_cast<hash_type>(static_cast<long_type>(static_cast<long_type>(acc)
                        + (static_cast<long_type>(input) * static_cast<long_type>(primes[1])))), 13
                    ) * static_cast<long_type>(primes[0])
                );
            }

            constexpr static hash_type convergence(hash_type acc0, hash_type acc1, hash_type acc2, hash_type acc3)
            {
                return static_cast<hash_type>(
                    static_cast<long_type>(rotl(acc0, 1))
                    + static_cast<long_type>(rotl(acc1, 7))
                    + static_cast<long_type>(rotl(acc2, 12))
                    + static_cast<long_type>(rotl(acc3, 18))
                );
            }

            constexpr static hash_type final_mix(hash_type acc)
            {
                acc = static_cast<hash_type>((acc ^ (acc >> 15)) * static_cast<long_type>(primes[1]));
                acc = static_cast<hash_type>((acc ^ (acc >> 13)) * static_cast<long_type>(primes[2]));
                acc = acc ^ (acc >> 16);
                return acc;
            }
        };

        template<>
        class xxhasher<uint64_t>
        {
        public:
            using hash_type = std::uint64_t;
            using long_type = std::uintmax_t;

            constexpr static hash_type hash(const char* input, std::size_t len, hash_type seed)
            {
                if (input == nullptr)
                    len = 0;

                const char* src{ input };
                const char* end{ src + len };
                hash_type acc = 0;

                // step 1.
                if (len >= 32)
                {
                    const char* limit{ end - 32 };
                    hash_type acc0 = static_cast<hash_type>(seed + static_cast<long_type>(primes[0] + static_cast<long_type>(primes[1])));
                    hash_type acc1 = static_cast<hash_type>(seed + static_cast<long_type>(primes[1]));
                    hash_type acc2 = seed + 0;
                    hash_type acc3 = seed - primes[0];

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
                    acc = static_cast<hash_type>(seed + static_cast<long_type>(primes[4]));
                }

                // step 4.
                acc += len;

                // step 5.
                if (len >= 8)
                {
                    const char* limit{ end - 8 };
                    while (src <= limit)
                    {
                        acc = static_cast<hash_type>(
                            rotl<std::uint64_t>(acc ^ round(0, read<std::uint64_t>(src)), 27)
                            * static_cast<long_type>(primes[0]) + static_cast<long_type>(primes[3]));
                        src += sizeof(std::uint64_t);
                    }
                }

                if (len >= 4)
                {
                    const char* limit{ end - 4 };
                    while (src <= limit)
                    {
                        acc = static_cast<hash_type>(
                            rotl<hash_type>(acc ^ static_cast<hash_type>((read<std::uint32_t>(src) * static_cast<long_type>(primes[0]))), 23)
                            * static_cast<long_type>(primes[1]) + static_cast<long_type>(primes[2]));
                        src += sizeof(std::uint32_t);
                    }
                }

                if (input != nullptr)
                {
                    while (src < end)
                    {
                        acc = static_cast<hash_type>(
                            rotl<hash_type>(acc ^ (read<std::uint8_t>(src) * static_cast<long_type>(primes[4])), 11)
                            * static_cast<long_type>(primes[0]));
                        ++src;
                    }
                }

                // step 6.
                acc = final_mix(acc);

                return acc;
            }
        private:
            constexpr static std::array<hash_type, 5> primes{{
                11400714785074694791ULL, 14029467366897019727ULL, 1609587929392839161ULL, 9650029242287828579ULL, 2870177450012600261ULL
            }};

            constexpr static hash_type round(hash_type acc, hash_type input)
            {
                acc = static_cast<hash_type>(acc + (static_cast<long_type>(input) * static_cast<long_type>(primes[1])));
                acc = rotl(acc, 31);
                return static_cast<hash_type>(acc * static_cast<long_type>(primes[0]));
            }

            constexpr static hash_type mearge_accumulator(hash_type acc, hash_type acc_n)
            {
                return static_cast<hash_type>(
                    ((acc ^ round(0, acc_n)) * static_cast<long_type>(primes[0]))
                    + static_cast<long_type>(primes[3])
                );
            }

            constexpr static hash_type convergence(hash_type acc0, hash_type acc1, hash_type acc2, hash_type acc3)
            {
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

            constexpr static hash_type final_mix(hash_type acc) {
                acc = static_cast<hash_type>((acc ^ (acc >> 33)) * static_cast<long_type>(primes[1]));
                acc = static_cast<hash_type>((acc ^ (acc >> 29)) * static_cast<long_type>(primes[2]));
                acc = (acc ^ (acc >> 32));
                return acc;
            }
        };

        template<typename T>
        constexpr T xxhash(const char* buf, size_t len, T seed)
        {
            static_assert(std::is_same<T, uint32_t>() || std::is_same<T, uint64_t>(), "HasSh type is not supported");
            return xxhasher<T>::hash(buf, len, seed);
        }
    }

    constexpr hash32_t xxhash32(const char* buf, hash32_t seed = detail::xxhash::constant<uint32_t>::seed)
    {
        return detail::xxhash::xxhash<uint32_t>(buf, pride::ct::strlen(buf), seed);
    }

    constexpr hash64_t xxhash64(const char* buf, hash64_t seed = detail::xxhash::constant<uint64_t>::seed)
    {
        return detail::xxhash::xxhash<uint64_t>(buf, pride::ct::strlen(buf), seed);
    }
}

constexpr pride::hash32_t operator "" _xxhash32(const char* key, const size_t len)
{
    return ::pride::hash::xxhash32(key, len);
}

constexpr pride::hash64_t operator "" _xxhash64(const char* key, const size_t len)
{
    return ::pride::hash::xxhash64(key, len);
}
