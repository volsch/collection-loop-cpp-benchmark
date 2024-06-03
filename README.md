# C++ Collection Loop Benchmark

Benchmark for different types of C++ collection loops. The benchmark uses C++23 and
is based on [Google Benchmark framework](https://github.com/google/benchmark).

Build:

```bash
cmake -B build -S .
cmake --build build
```

Run:

```bash
build/collection_loop_benchmark --benchmark_min_warmup_time=2 --benchmark_min_time=2s --benchmark_repetitions=3
```
