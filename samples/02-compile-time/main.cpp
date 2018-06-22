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

    cout << "type_name<foo> = " << pride::ct::type_name<foo>.str << '\n';
    cout << "decay_type_name<foo> = " << pride::ct::decay_type_name<foo> << '\n';
    cout << "type_hash<foo> = " << pride::ct::type_hash<foo> << '\n' << '\n';

    cout << "type_name<const char*> = " << pride::ct::type_name<const char*>.str << '\n';
    cout << "decay_type_name<const char*> = " << pride::ct::decay_type_name<const char*> << '\n';
    cout << "type_hash<const char*> = " << pride::ct::type_hash<const char*> << '\n' << '\n';

    cout << "type_name<const foo&> = " << pride::ct::type_name<const foo&>.str << '\n';
    cout << "decay_type_name<const foo&> = " << pride::ct::decay_type_name<const foo&> << '\n';
    cout << "type_hash<const foo&> = " << pride::ct::type_hash<const foo&> << '\n' << '\n';

    // Testing compile time checking of value_name
    constexpr int value = 0;
    cout << pride::ct::value_name<value> << '\n';
    // static_assert(pride::ct::value_name<value> == pride::ct::type_name<int>);

    // checking ct::string to see if find is working correctly
    // pride::ct::string str = "this is some compile time string";
    // cout << str << '\n';
    // cout << "find: " << str.find("compile") << " rfind: " << str.rfind("compile")
    //      << " substr: " << pride::ct::strlen("compile") << " diff: " << str.rfind("compile") - str.find("compile") << '\n';

    return 0;
}
