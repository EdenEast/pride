
#ifndef CATCH_CONFIG_RUNNER
#define CATCH_CONFIG_RUNNER
#endif

#if defined(PRI_OS_WINDOWS)
#define CATCH_CONFIG_COLOUR_WINDOWS
#endif

#include <test.hpp>

int main(int argc, char* argv[])
{
    int result = Catch::Session().run(argc, argv);

#if defined(PRI_OS_WINDOWS)
    system("pause");
#endif
    return result;
}

