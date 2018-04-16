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

    cout << "type_id<foo>().name() = " << ct::type_id<foo>().name() << '\n';
    cout << "type_id<foo>().hash() = " << ct::type_id<foo>().hash() << "\n\n";

    cout << "unnamed_type_id<foo>().hash() = " << ct::unnaamed_type_id<foo>().hash() << '\n';
    cout << "type_id<foo>().hash() == unnamed_type_id<foo>().hash() -> (" << ct::type_id<foo>().hash() << " == " << ct::unnaamed_type_id<foo>().hash()
         << ") -> " << (ct::type_id<foo>().hash() == ct::unnaamed_type_id<foo>().hash() ? "true" : "false") << "\n\n";

    foo f;
    cout << "type_id(f) = " << ct::type_id(f).name() << '\n';
    cout << "type_id(f.i) = " << ct::type_id(f.i).name() << '\n';
    cout << "type_id(f.c) = " << ct::type_id(f.c).name() << "\n\n";

    cout << "ct::type_id<ct::static_value<int, foo::i>>() = " << ct::type_id<ct::static_value<int, foo::i>>().name() << '\n';
    return 0;
}
