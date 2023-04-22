cmake_minimum_required(VERSION 3.18)
project(benchmark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-march=native")
add_executable(rng generate_random_numbers.cc)
target_compile_options(rng PUBLIC "-O2")

add_executable(sum perf.cc)
target_compile_options(sum PUBLIC "-mavx512f" "-mavx512dq" "-O2")
# g++ -g perf.cc -std=c++17 -isystem benchmark/include -Lbenchmark/build/src -lbenchmark -lpthread -o mybenchmark
target_include_directories(sum PUBLIC "benchmark/include")
target_link_directories(sum PUBLIC "benchmark/build/src")
target_link_libraries(sum benchmark pthread)

add_executable(sort sort.cc)
# g++ -g -O0 sort.cc -std=c++17 -mavx512f -mavx512dq -isystem benchmark/include -Lbenchmark/build/src -lbenchmark -lpthread -o mybenchmark
target_compile_options(sort PUBLIC "-mavx512f" "-mavx512dq" "-O2")
target_link_directories(sort PUBLIC "benchmark/build/src")
target_link_libraries(sort benchmark pthread)
target_include_directories(sort PUBLIC "benchmark/include")

