
#include <benchmark/benchmark.h>
#include <pride/pride.hpp>
#include <iostream>
#include <cstring>

constexpr const char* test_string = "This is a long string to make sure it is not short string optimized!";
constexpr size_t test_size = pride::ct::strlen(test_string);

static void crc32(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(
            pride::hash::crc32(test_string, test_size)
        );
    }
}
BENCHMARK(crc32)->Unit(benchmark::kNanosecond);

static void fasthash32(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(
            pride::hash::fasthash32(test_string, test_size)
        );
    }
}
BENCHMARK(fasthash32)->Unit(benchmark::kNanosecond);

static void fasthash64(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(
            pride::hash::fasthash64(test_string, test_size)
        );
    }
}
BENCHMARK(fasthash64)->Unit(benchmark::kNanosecond);

static void fnv1a32(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(
            pride::hash::fnv1a32(test_string, test_size)
        );
    }
}
BENCHMARK(fnv1a32)->Unit(benchmark::kNanosecond);

static void fnv1a64(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(
            pride::hash::fnv1a64(test_string, test_size)
        );
    }
}
BENCHMARK(fnv1a64)->Unit(benchmark::kNanosecond);

static void mm332(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(
            pride::hash::mm332(test_string, test_size)
        );
    }
}
BENCHMARK(mm332)->Unit(benchmark::kNanosecond);

static void mm364(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(
            pride::hash::mm364(test_string, test_size)
        );
    }
}
BENCHMARK(mm364)->Unit(benchmark::kNanosecond);

static void xxhash32(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(
            pride::hash::xxhash32(test_string, test_size)
        );
    }
}
BENCHMARK(xxhash32)->Unit(benchmark::kNanosecond);

static void xxhash64(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(
            pride::hash::xxhash64(test_string, test_size)
        );
    }
}
BENCHMARK(xxhash64)->Unit(benchmark::kNanosecond);

BENCHMARK_MAIN();
