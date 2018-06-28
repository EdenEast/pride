
#include <pride/ct.hpp>
#include <pride/hash.hpp>
#include <iostream>
#include <cstring>
#include <string>
#include <cstdint>
#include <stdlib.h>
#include <random>
#include <time.h>

int main()
{
    using namespace pride;
    using std::cout;

    const char* sample = "here is a test string";

    cout << "crc\n";
    cout << "-----------------------------------------------\n";
    cout << "crc32:   0x" << std::hex << pride::hash::crc32(sample, strlen(sample)) << '\n';
    cout << "-----------------------------------------------\n\n";

    cout << "fast hash\n";
    cout << "-----------------------------------------------\n";
    cout << "fasthash:   0x" << std::hex << pride::hash::fasthash(sample, strlen(sample)) << '\n';
    cout << "fasthash32: 0x" << std::hex << pride::hash::fasthash32(sample, strlen(sample)) << '\n';
    cout << "fasthash64: 0x" << std::hex << pride::hash::fasthash64(sample, strlen(sample)) << '\n';
    cout << "-----------------------------------------------\n\n";

    cout << "fnv1a\n";
    cout << "-----------------------------------------------\n";
    cout << "fnv1a:   0x" << std::hex << pride::hash::fnv1a(sample, strlen(sample)) << '\n';
    cout << "fnv1a32: 0x" << std::hex << pride::hash::fnv1a32(sample, strlen(sample)) << '\n';
    cout << "fnv1a64: 0x" << std::hex << pride::hash::fnv1a64(sample, strlen(sample)) << '\n';
    cout << "-----------------------------------------------\n\n";

    cout << "mm3\n";
    cout << "-----------------------------------------------\n";
    cout << "mm3:   0x" << std::hex << pride::hash::mm3(sample, strlen(sample)) << '\n';
    cout << "mm332: 0x" << std::hex << pride::hash::mm332(sample, strlen(sample)) << '\n';
    cout << "mm364: 0x" << std::hex << pride::hash::mm364(sample, strlen(sample)) << '\n';
    cout << "-----------------------------------------------\n\n";

    cout << "xxhash\n";
    cout << "-----------------------------------------------\n";
    cout << "xxhash:   0x" << std::hex << pride::hash::xxhash(sample, strlen(sample)) << '\n';
    cout << "xxhash32: 0x" << std::hex << pride::hash::xxhash32(sample, strlen(sample)) << '\n';
    cout << "xxhash64: 0x" << std::hex << pride::hash::xxhash64(sample, strlen(sample)) << '\n';
    cout << "-----------------------------------------------\n\n";
}
