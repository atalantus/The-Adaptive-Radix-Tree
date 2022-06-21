cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)

add_executable (Benchmark benchmark.cpp trie.cpp trie.h)
