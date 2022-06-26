cmake_minimum_required(VERSION 3.8)

project(Benchmark)

set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_NAME Benchmark)

add_subdirectory(data_structures)

add_executable(${EXECUTABLE_NAME} main.cpp benchmark.h benchmark.cpp clock.h)

target_link_libraries(${EXECUTABLE_NAME} art trie)
