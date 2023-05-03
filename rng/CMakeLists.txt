find_package(gflags REQUIRED)

add_executable(rng generate_random_numbers.cc)
target_link_libraries(rng gflags)
