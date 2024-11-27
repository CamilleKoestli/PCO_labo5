#include <benchmark/benchmark.h>
#include <iostream>

#include "quicksort.h"
#include "utils.h"

/**
 * @brief BenchmarkQuicksort generates a sequence of specified size and uses N threads to sort it with Quicksort.
 * @param size of the sequence to sort
 * @param nthreads number of threads to use to sort the sequence
 */
void BenchmarkQuicksort(int size, int nthreads)
{
    Quicksort<int> sorter(nthreads);
    int seed = 23;
    std::vector<int> test = generateSequence(size, seed);
    sorter.sort(test);
}

static void BM_QS_MANYTHREADS(benchmark::State& state) {
    int nthreads = state.range(0);
    int size = 5000000;
    for (auto _ : state) {
        BenchmarkQuicksort(size, nthreads);
    }
}

BENCHMARK(BM_QS_MANYTHREADS)->Arg(1)->Arg(2)->Arg(4)->Arg(8)->Arg(16)->UseRealTime();

BENCHMARK_MAIN();
