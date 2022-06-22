cmake_minimum_required(VERSION 3.8)

project(Benchmark)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(data_structures)
add_executable(Benchmark benchmark.cpp)
