
#include <pride/ct.hpp>
#include <iostream>
#include <string>

constexpr pride::hash_t hash_value(const char* value, size_t len)
{
    return pride::hash::fnv1a(value, len);
}

int main()
{
    using namespace pride;
    using std::cout;

    const char* sample = "here is a test string";

    cout << "crc32\n";
    cout << "-----------------------------------------------\n";
    cout << "crc32: " << hash::crc32(sample) << '\n';
    cout << "_crc32: " << "here is a test string"_crc32 << '\n';
    cout << "-----------------------------------------------\n\n";

    cout << "djb2a\n";
    cout << "-----------------------------------------------\n";
    cout << "djb2a: " << hash::djb2a(sample) << '\n';
    cout << "_djb2a: " << "here is a test string"_djb2a << '\n';
    cout << "-----------------------------------------------\n\n";

    cout << "fnv1a\n";
    cout << "-----------------------------------------------\n";
    cout << "fnv1a: " << hash::fnv1a(sample, pride::ct::strlen(sample)) << '\n';
    cout << "fnv1a32: " << hash::fnv1a32(sample, pride::ct::strlen(sample)) << '\n';
    cout << "fnv1a64: " << hash::fnv1a64(sample, pride::ct::strlen(sample)) << '\n';
    cout << "_fnv1a: " << "here is a test string"_fnv1a << '\n';
    cout << "_fnv1a32: " << "here is a test string"_fnv1a32 << '\n';
    cout << "_fnv1a64: " << "here is a test string"_fnv1a64 << '\n';
    cout << "-----------------------------------------------\n\n";

    cout << "mm3_32\n";
    cout << "-----------------------------------------------\n";
    cout << "mm3 32: " << hash::mm3_32(sample) << '\n';
    cout << "_mm3_32: " << "here is a test string"_mm3_32 << '\n';
    cout << "-----------------------------------------------\n\n";

    cout << "xxhash\n";
    cout << "-----------------------------------------------\n";
    cout << "xxhash32: " << hash::xxhash32(sample) << '\n';
    cout << "xxhash64: " << hash::xxhash64(sample) << '\n';
    cout << "_xxhash32: " << "here is a test string"_xxhash32 << '\n';
    cout << "_xxhash64: " << "here is a test string"_xxhash64 << '\n';
    cout << "-----------------------------------------------\n\n";
}
