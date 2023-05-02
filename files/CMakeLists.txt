find_package(gflags REQUIRED)

add_executable(files generate_files.cc)
target_link_libraries(files gflags)
