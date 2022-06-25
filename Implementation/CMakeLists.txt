cmake_minimum_required(VERSION 3.8)

project(Benchmark)

set(CMAKE_CXX_STANDARD 20)

set(EXECUTABLE_NAME Benchmark)

add_executable(${EXECUTABLE_NAME} main.cpp benchmark.cpp benchmark.h)

add_subdirectory(data_structures)

target_link_libraries(${EXECUTABLE_NAME} art trie)