cmake_minimum_required(VERSION 3.20)
project(benchmark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-march=native")
add_executable(rng generate_random_numbers.cc)


