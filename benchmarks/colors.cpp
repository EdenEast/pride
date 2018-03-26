#include <celero/Celero.h>
#include <pride/pride.hpp>

static int base_line()
{
    std::cout << 1234 << '\n';
    return 0;
}

static int color_line()
{
    using namespace pride::console;
    std::cout << fg_t::red << 1234 << fg_t::reset << '\n';
}

BASELINE(Consle_Colors, Baseline, 30, 10000)
{
    std::streambuf* old = std::cout.rdbuf();
    defer{ std::cout.rdbuf(old); };
    std::stringstream ss;
    std::cout.rdbuf(ss.rdbuf());
    celero::DoNotOptimizeAway(base_line());
}

BENCHMARK(Consle_Colors, ColorPrint, 30, 10000)
{
    std::streambuf* old = std::cout.rdbuf();
    defer{ std::cout.rdbuf(old); };
    std::stringstream ss;
    std::cout.rdbuf(ss.rdbuf());
    celero::DoNotOptimizeAway(base_line());
}

