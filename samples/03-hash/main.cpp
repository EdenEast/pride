
#include <pride/ct.hpp>
#include <iostream>

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
    cout << "fnv1a 32: " << hash::fnv1a(sample, pride::ct::strlen(sample)) << '\n';
    cout << "fnv1a 64: " << hash::fnv1a64(sample, pride::ct::strlen(sample)) << '\n';
    cout << "_fnv1a_32: " << "here is a test string"_fnv1a_32 << '\n';
    cout << "_fnv1a_64: " << "here is a test string"_fnv1a_64 << '\n';
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
    cout << "-----------------------------------------------\n\n";
}
