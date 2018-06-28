#define PRI_USE_CT_TEST
#include <pride/ct.hpp>
#include <iostream>

struct foo
{
    constexpr static int i = 1337;
    constexpr static const char* c = "this is foo";
};

int main()
{
    using namespace std;
    using namespace pride;

    constexpr const char* sample = "Here is a sample string";

    cout << "type_name<foo> = " << pride::ct::type_name<foo>.str << '\n';
    cout << "decay_type_name<foo> = " << pride::ct::decay_type_name<foo> << '\n';
    cout << "type_hash<foo> = " << pride::ct::type_hash<foo> << '\n' << '\n';

    cout << "type_name<const char*> = " << pride::ct::type_name<const char*>.str << '\n';
    cout << "decay_type_name<const char*> = " << pride::ct::decay_type_name<const char*> << '\n';
    cout << "type_hash<const char*> = " << pride::ct::type_hash<const char*> << '\n' << '\n';

    cout << "type_name<const foo&> = " << pride::ct::type_name<const foo&>.str << '\n';
    cout << "decay_type_name<const foo&> = " << pride::ct::decay_type_name<const foo&> << '\n';
    cout << "type_hash<const foo&> = " << pride::ct::type_hash<const foo&> << '\n' << '\n';

    cout << "crc32\n";
    cout << "-----------------------------------------------\n";
    cout << "crc32: " << pride::ct::hash::crc32(sample) << '\n';
    cout << "_crc32: " << "here is a test string"_crc32 << '\n';
    cout << "-----------------------------------------------\n\n";

    cout << "djb2a\n";
    cout << "-----------------------------------------------\n";
    cout << "djb2a: " << pride::ct::hash::djb2a(sample) << '\n';
    cout << "_djb2a: " << "here is a test string"_djb2a << '\n';
    cout << "-----------------------------------------------\n\n";

    cout << "fnv1a\n";
    cout << "-----------------------------------------------\n";
    cout << "fnv1a: " << pride::ct::hash::fnv1a(sample, pride::ct::strlen(sample)) << '\n';
    cout << "fnv1a32: " << pride::ct::hash::fnv1a32(sample, pride::ct::strlen(sample)) << '\n';
    cout << "fnv1a64: " << pride::ct::hash::fnv1a64(sample, pride::ct::strlen(sample)) << '\n';
    cout << "_fnv1a: " << "here is a test string"_fnv1a << '\n';
    cout << "_fnv1a32: " << "here is a test string"_fnv1a32 << '\n';
    cout << "_fnv1a64: " << "here is a test string"_fnv1a64 << '\n';
    cout << "-----------------------------------------------\n\n";

    cout << "mm3_32\n";
    cout << "-----------------------------------------------\n";
    cout << "mm3 32: " << pride::ct::hash::mm3_32(sample, pride::ct::strlen(sample)) << '\n';
    cout << "_mm3_32: " << "here is a test string"_mm3_32 << '\n';
    cout << "-----------------------------------------------\n\n";

    cout << "xxhash\n";
    cout << "-----------------------------------------------\n";
    cout << "xxhash:    " << pride::ct::hash::xxhash(sample, pride::ct::strlen(sample)) << '\n';
    cout << "xxhash32:  " << pride::ct::hash::xxhash32(sample, pride::ct::strlen(sample)) << '\n';
    cout << "xxhash64:  " << pride::ct::hash::xxhash64(sample, pride::ct::strlen(sample)) << '\n';
    cout << "_xxhash:   " << "here is a test string"_xxhash << '\n';
    cout << "_xxhash32: " << "here is a test string"_xxhash32 << '\n';
    cout << "_xxhash64: " << "here is a test string"_xxhash64 << '\n';
    cout << "-----------------------------------------------\n\n";

    return 0;
}
