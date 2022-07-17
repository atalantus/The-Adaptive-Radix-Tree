#!/bin/sh

# Execute a set of different benchmarks for size 1

DIR_PATH="./out/build/gcc-x64-release"

# Insert
$DIR_PATH/Benchmark -b insert -s 1 -i 10 -d
echo ""
$DIR_PATH/Benchmark -b insert -s 1 -i 10
echo ""

# Search
$DIR_PATH/Benchmark -b search -s 1 -i 10 -d
echo ""
$DIR_PATH/Benchmark -b search -s 1 -i 10
echo ""

# Range Search
$DIR_PATH/Benchmark -b range_search -s 1 -i 5 -d
echo ""
$DIR_PATH/Benchmark -b range_search -s 1 -i 5