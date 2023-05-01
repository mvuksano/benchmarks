cmake_minimum_required(VERSION 3.18)
project(benchmark)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-march=native -Wall -Wextra -O2 -g)
add_executable(rng generate_random_numbers.cc)

add_subdirectory(sum)
add_subdirectory(files)
add_subdirectory(sort)
