#define PRIDE_USE_CT_TEST
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

    cout << "fnv1a\n";
    cout << "-----------------------------------------------\n";
    cout << "fnv1a: " << pride::ct::fnv1a(sample, pride::ct::strlen(sample)) << '\n';
    cout << "fnv1a32: " << pride::ct::fnv1a32(sample, pride::ct::strlen(sample)) << '\n';
    cout << "fnv1a64: " << pride::ct::fnv1a64(sample, pride::ct::strlen(sample)) << '\n';
    cout << "_fnv1a: " << "here is a test string"_fnv1a << '\n';
    cout << "_fnv1a32: " << "here is a test string"_fnv1a32 << '\n';
    cout << "_fnv1a64: " << "here is a test string"_fnv1a64 << '\n';
    cout << "-----------------------------------------------\n\n";

    //static_assert(pride::ct::fnv1a("This should match") == "This should match"_fnv1a);

    cout << pride::ct::fnv1a("This should match") << " == " << "This should match"_fnv1a << '\n';
    return 0;
}
