
#include <pride/ct.hpp>
#include <pride/hash.hpp>
#include <iostream>
#include <cstring>
#include <string>

constexpr pride::hash_t hash_value(const char* value, size_t len)
{
    return pride::ct::hash::fnv1a(value, len);
}

int main()
{
    using namespace pride;
    using std::cout;

    const char* sample = "here is a test string";

    cout << "fast hash\n";
    cout << "-----------------------------------------------\n";
    cout << "fasthash:   " << pride::hash::fasthash(sample, strlen(sample)) << '\n';
    cout << "fasthash32: " << pride::hash::fasthash32(sample, strlen(sample)) << '\n';
    cout << "fasthash64: " << pride::hash::fasthash64(sample, strlen(sample)) << '\n';
    cout << "-----------------------------------------------\n\n";

    cout << "xxhash\n";
    cout << "-----------------------------------------------\n";
    cout << "xxhash:   " << pride::hash::xxhash(sample, strlen(sample)) << '\n';
    cout << "xxhash32: " << pride::hash::xxhash32(sample, strlen(sample)) << '\n';
    cout << "xxhash64: " << pride::hash::xxhash64(sample, strlen(sample)) << '\n';
    cout << "-----------------------------------------------\n\n";


    // cout << "crc32\n";
    // cout << "-----------------------------------------------\n";
    // cout << "crc32: " << ct::hash::crc32(sample) << '\n';
    // cout << "_crc32: " << "here is a test string"_crc32 << '\n';
    // cout << "-----------------------------------------------\n\n";

    // cout << "djb2a\n";
    // cout << "-----------------------------------------------\n";
    // cout << "djb2a: " << ct::hash::djb2a(sample) << '\n';
    // cout << "_djb2a: " << "here is a test string"_djb2a << '\n';
    // cout << "-----------------------------------------------\n\n";

    // cout << "fnv1a\n";
    // cout << "-----------------------------------------------\n";
    // cout << "fnv1a: " << ct::hash::fnv1a(sample, ct::strlen(sample)) << '\n';
    // cout << "fnv1a32: " << ct::hash::fnv1a32(sample, ct::strlen(sample)) << '\n';
    // cout << "fnv1a64: " << ct::hash::fnv1a64(sample, ct::strlen(sample)) << '\n';
    // cout << "_fnv1a: " << "here is a test string"_fnv1a << '\n';
    // cout << "_fnv1a32: " << "here is a test string"_fnv1a32 << '\n';
    // cout << "_fnv1a64: " << "here is a test string"_fnv1a64 << '\n';
    // cout << "-----------------------------------------------\n\n";

    // cout << "mm3_32\n";
    // cout << "-----------------------------------------------\n";
    // cout << "mm3 32: " << ct::hash::mm3_32(sample) << '\n';
    // cout << "_mm3_32: " << "here is a test string"_mm3_32 << '\n';
    // cout << "-----------------------------------------------\n\n";

    // cout << "xxhash\n";
    // cout << "-----------------------------------------------\n";
    // cout << "xxhash32: " << ct::hash::xxhash32(sample) << '\n';
    // cout << "xxhash64: " << ct::hash::xxhash64(sample) << '\n';
    // cout << "_xxhash32: " << "here is a test string"_xxhash32 << '\n';
    // cout << "_xxhash64: " << "here is a test string"_xxhash64 << '\n';
    // cout << "-----------------------------------------------\n\n";
}
