/*
 * Copyright (c) 2024, Volker Schmidt
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice,this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <benchmark/benchmark.h>

#include <algorithm>
#include <cstdlib>
#include <new>
#include <vector>

static unsigned long long allocationCount = 0;
static unsigned long long allocatedBytes = 0;

void *operator new(std::size_t size) {
  allocationCount++;
  allocatedBytes += size;
  return std::malloc(size);
}

void operator delete(void *p) noexcept { std::free(p); }

void *operator new[](std::size_t size) {
  allocationCount++;
  allocatedBytes += size;
  return std::malloc(size);
}

void operator delete[](void *p) noexcept { std::free(p); }

class CollectionLoopFixture : public benchmark::Fixture {
 protected:
  std::size_t loopIterations;
  std::vector<int> values;

 public:
  void SetUp(::benchmark::State &state) override {
    loopIterations = state.range(0);

    values.clear();
    values.reserve(loopIterations);
    for (std::size_t i = 0; i < loopIterations; i++) {
      values.push_back(i);
    }
  }

  void TearDown(::benchmark::State &state) override { values.clear(); }
};

BENCHMARK_DEFINE_F(CollectionLoopFixture, loop)(benchmark::State &state) {
  allocationCount = 0;
  allocatedBytes = 0;

  for (auto _ : state) {
    const auto &values = this->values;
    const std::size_t size = values.size();
    std::vector<int> result;
    result.reserve(size);

    for (std::size_t i = 0; i < size; i++) {
      const auto value = values.at(i);
      if (value % 2 == 0) {
        result.push_back(value);
      }
    }

    benchmark::DoNotOptimize(result);
    benchmark::ClobberMemory();
  }

  const auto finalAllocationCount = allocationCount;
  const auto finalAllocatedBytes = allocatedBytes;
  state.counters["AllocCount"] = benchmark::Counter(
      finalAllocationCount, benchmark::Counter::kAvgIterations);
  state.counters["AllocBytes"] = benchmark::Counter(
      finalAllocatedBytes, benchmark::Counter::kAvgIterations);
}
BENCHMARK_REGISTER_F(CollectionLoopFixture, loop)
    ->Arg(0)
    ->Arg(1)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000);

BENCHMARK_DEFINE_F(CollectionLoopFixture, enhancedLoop)
(benchmark::State &state) {
  allocationCount = 0;
  allocatedBytes = 0;

  for (auto _ : state) {
    const auto &values = this->values;
    std::vector<int> result;
    result.reserve(values.size());

    for (const auto value : values) {
      if (value % 2 == 0) {
        result.push_back(value);
      }
    }

    benchmark::DoNotOptimize(result);
    benchmark::ClobberMemory();
  }

  const auto finalAllocationCount = allocationCount;
  const auto finalAllocatedBytes = allocatedBytes;
  state.counters["AllocCount"] = benchmark::Counter(
      finalAllocationCount, benchmark::Counter::kAvgIterations);
  state.counters["AllocBytes"] = benchmark::Counter(
      finalAllocatedBytes, benchmark::Counter::kAvgIterations);
}
BENCHMARK_REGISTER_F(CollectionLoopFixture, enhancedLoop)
    ->Arg(0)
    ->Arg(1)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000);

BENCHMARK_DEFINE_F(CollectionLoopFixture, iterator)(benchmark::State &state) {
  allocationCount = 0;
  allocatedBytes = 0;

  for (auto _ : state) {
    const auto &values = this->values;
    std::vector<int> result;
    result.reserve(values.size());

    for (auto it = values.begin(); it != values.end(); ++it) {
      const auto value = *it;
      if (value % 2 == 0) {
        result.push_back(value);
      }
    }

    benchmark::DoNotOptimize(result);
    benchmark::ClobberMemory();
  }

  const auto finalAllocationCount = allocationCount;
  const auto finalAllocatedBytes = allocatedBytes;
  state.counters["AllocCount"] = benchmark::Counter(
      finalAllocationCount, benchmark::Counter::kAvgIterations);
  state.counters["AllocBytes"] = benchmark::Counter(
      finalAllocatedBytes, benchmark::Counter::kAvgIterations);
}
BENCHMARK_REGISTER_F(CollectionLoopFixture, iterator)
    ->Arg(0)
    ->Arg(1)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000);

BENCHMARK_DEFINE_F(CollectionLoopFixture, iteratorConstEnd)
(benchmark::State &state) {
  allocationCount = 0;
  allocatedBytes = 0;

  for (auto _ : state) {
    const auto &values = this->values;
    std::vector<int> result;
    result.reserve(values.size());

    const auto end = values.end();
    for (auto it = values.begin(); it != end; ++it) {
      const auto value = *it;
      if (value % 2 == 0) {
        result.push_back(value);
      }
    }

    benchmark::DoNotOptimize(result);
    benchmark::ClobberMemory();
  }

  const auto finalAllocationCount = allocationCount;
  const auto finalAllocatedBytes = allocatedBytes;
  state.counters["AllocCount"] = benchmark::Counter(
      finalAllocationCount, benchmark::Counter::kAvgIterations);
  state.counters["AllocBytes"] = benchmark::Counter(
      finalAllocatedBytes, benchmark::Counter::kAvgIterations);
}
BENCHMARK_REGISTER_F(CollectionLoopFixture, iteratorConstEnd)
    ->Arg(0)
    ->Arg(1)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000);

BENCHMARK_DEFINE_F(CollectionLoopFixture, forEach)(benchmark::State &state) {
  allocationCount = 0;
  allocatedBytes = 0;

  for (auto _ : state) {
    const auto &values = this->values;
    std::vector<int> result;
    result.reserve(values.size());

    std::for_each(values.begin(), values.end(), [&result](int value) {
      if (value % 2 == 0) {
        result.push_back(value);
      }
    });

    benchmark::DoNotOptimize(result);
    benchmark::ClobberMemory();
  }

  const auto finalAllocationCount = allocationCount;
  const auto finalAllocatedBytes = allocatedBytes;
  state.counters["AllocCount"] = benchmark::Counter(
      finalAllocationCount, benchmark::Counter::kAvgIterations);
  state.counters["AllocBytes"] = benchmark::Counter(
      finalAllocatedBytes, benchmark::Counter::kAvgIterations);
}
BENCHMARK_REGISTER_F(CollectionLoopFixture, forEach)
    ->Arg(0)
    ->Arg(1)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000);

BENCHMARK_MAIN();
