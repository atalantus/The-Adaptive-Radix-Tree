cmake_minimum_required(VERSION 3.8)

project(Benchmark)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -msse2")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(EXECUTABLE_NAME Benchmark)

add_subdirectory(data_structures)

add_executable(${EXECUTABLE_NAME} main.cpp benchmark.h util.h)

target_link_libraries(${EXECUTABLE_NAME} art trie mtrie htrie sorted_list hash_table rbtree)
