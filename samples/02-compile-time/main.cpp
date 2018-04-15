#define PRI_USE_CT_TEST
#include <pride/ct.hpp>
#include <iostream>

struct something {};

struct foo { int i = 10010; };

int main()
{
    using namespace std;
    using namespace pride::ct;

    cout << type_id<something>().name() << endl;
    cout << unnaamed_type_id<something>().hash() << endl;
    cout << (type_id<something>().hash() == unnaamed_type_id<something>().hash() ? "true" : "false") << endl;
    cout << type_id(cout).name() << endl;

    return 0;
}
