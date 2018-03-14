#include <celero/Celero.h>
#include <pride/pride.hpp>

CELERO_MAIN

BASELINE(Guid, Baseline, 30, 10000)
{
    celero::DoNotOptimizeAway(pride::guid_t::create());
}
