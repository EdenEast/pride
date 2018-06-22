// https://github.com/masyos/xxhash_cx/blob/master/include/xxhash_cx.h
#pragma once

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
        class xxhash {};

        template<>
        class xxhash<uint32_t>
        {
        public:
            using hash_type = std::uint32_t;
            using long_type = std::uint64_t;

            constexpr static hash_type xxh(const char* input, std::size_t len, hash_type seed)
            {
                if (input == nullptr)
                    len = 0;
                return len;
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
    }
}
