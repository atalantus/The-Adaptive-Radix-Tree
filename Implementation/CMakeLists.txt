cmake_minimum_required (VERSION 3.8)

project ("Implementation")

set(CMAKE_CXX_STANDARD 20)

# Include sub-projects.
add_subdirectory ("ART")
add_subdirectory("Benchmark")
