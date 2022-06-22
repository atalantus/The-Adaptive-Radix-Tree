cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)

add_executable(art art.h art.cpp node/node.h node/node4.h node/node4.cpp node/node16.h node/node16.cpp node/node48.h node/node48.cpp node/node256.h node/node256.cpp)
