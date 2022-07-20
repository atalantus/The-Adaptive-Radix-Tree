cmake_minimum_required(VERSION 3.8)

project(Benchmark)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Wall")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Ob3")
endif()

set(EXECUTABLE_NAME Benchmark)

add_subdirectory(data_structures)

add_executable(${EXECUTABLE_NAME} main.cpp benchmark.h util.h)

target_link_libraries(${EXECUTABLE_NAME} art trie mtrie htrie sorted_list hash_table rbtree)
