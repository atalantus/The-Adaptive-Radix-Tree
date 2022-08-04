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

add_subdirectory(data_structures)

add_executable("Benchmark" "benchmark/benchmark.cpp")
add_executable("Memory-Benchmark" "benchmark/benchmark.cpp")
add_executable("Test" "test/test.cpp")

target_link_libraries("Benchmark" data_structures)
target_link_libraries("Memory-Benchmark" data_structures)
target_link_libraries("Test" data_structures)

target_compile_definitions("Memory-Benchmark" PRIVATE TRACK_MEMORY)