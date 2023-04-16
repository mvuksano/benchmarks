g++ -g perf.cc -std=c++17 -isystem benchmark/include -Lbenchmark/build/src -lbenchmark -lpthread -o mybenchmark 

Install needed tools
apt-get install build-essential cmake git gdb


Clone markovuksanovic/benchmarks
https://github.com/mvuksano/benchmarks.git

Clone google/benchmark as subfolder
https://github.com/google/benchmark.git

Build:
g++ -g perf.cc -std=c++17 -isystem benchmark/include -Lbenchmark/build/src -lbenchmark -lpthread -o mybenchmark

Run:
./mybenchmark --benchmark_filter=".*Sum1024_x.*$" --benchmark_min_time=3s --benchmark_time_unit=ms


Example run:

```
markovuksanovic@instance-2:~/devel/benchmarks$ ./mybenchmark --benchmark_time_unit=us
2023-04-14T06:51:13+00:00
Running ./mybenchmark
Run on (8 X 2700 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x4)
  L1 Instruction 32 KiB (x4)
  L2 Unified 2048 KiB (x4)
  L3 Unified 107520 KiB (x1)
Load Average: 0.22, 0.05, 0.02
---------------------------------------------------------------
Benchmark                     Time             CPU   Iterations
---------------------------------------------------------------
AvxSum256                  4.65 us         4.65 us       150657
AvxSum1024                 18.5 us         18.5 us        37761
AvxSum1024_x_1024         18931 us        18931 us           37
Avx512Sum256              0.237 us        0.237 us      2947561
Avx512Sum1024             0.285 us        0.285 us      2455984
Avx512Sum1024_x_1024        128 us          128 us         5477
Avx512Sum4096             0.476 us        0.476 us      1473410
Sum256                    0.648 us        0.648 us      1078806
Sum1024                    2.70 us         2.70 us       259042
Sum4096                    10.9 us         10.9 us        64052
Sum1024_x_1024             2822 us         2822 us          248
Sum8192                    21.9 us         21.9 us        31976
```


# Random number generator

This program is used to generate a sequence of random numbers that can se used for benchmarking. Idea is to generate a sequence of numbers, store it into a file and use it to benchmark different scenarios.

Numnbers are stored in binary format. You tool like hexdump to view exact content. E.g.:

```
hexdump -e '1/8 "%016x" "\n"' nums
```
