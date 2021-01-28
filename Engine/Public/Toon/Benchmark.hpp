#ifndef TOON_BENCHMARK_HPP
#define TOON_BENCHMARK_HPP
/// Start Benchmark

#include <chrono>

#define ToonBenchmarkStart() \
    auto toonBenchClockStart = std::chrono::high_resolution_clock::now();

#define ToonBenchmarkEnd()                                                          \
    ToonLogPerf("Function: %s took %.3f millis\n", TOON_FUNCTION_NAME(),            \
        std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(  \
            std::chrono::high_resolution_clock::now() - toonBenchClockStart     \
        ).count());

/// End Benchmark

#endif // TOON_BENCHMARK_HPP