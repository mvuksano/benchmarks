cmake_minimum_required(VERSION 3.18)
project(benchmark)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-march=native -Wall -Wextra -O2 -g)
add_executable(rng generate_random_numbers.cc)

add_executable(sum sum.cc sum_naive.h sum_naive.cc)
target_include_directories(sum PUBLIC "benchmark/include")
target_link_directories(sum PUBLIC "benchmark/build/src")
target_link_libraries(sum benchmark pthread)
set_source_files_properties(sum_naive.cc PROPERTIES COMPILE_FLAGS "-mno-avx")
set_source_files_properties(sum.cc PROPERTIES COMPILE_FLAGS "-mavx512f -mavx512dq")

add_executable(files generate_files.cc)

add_executable(sort sort.cc)
target_compile_options(sort PUBLIC -mavx512f -mavx512dq)
target_link_directories(sort PUBLIC "benchmark/build/src")
target_link_libraries(sort benchmark pthread)
target_include_directories(sort PUBLIC "benchmark/include")

