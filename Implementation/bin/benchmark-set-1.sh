#!/bin/sh

# Execute a set of different benchmarks for size 1

DIR_PATH="../out/build/gcc-x64-release"

# Insert
$DIR_PATH/Benchmark -b insert -s 1 -i 100 -d --seed 1
echo
$DIR_PATH/Benchmark -b insert -s 1 -i 100 --seed 15000
echo

# Search
$DIR_PATH/Benchmark -b search -s 1 -i 100 -d --seed 30000
echo
$DIR_PATH/Benchmark -b search -s 1 -i 100 --seed 45000
echo

# Range Search
$DIR_PATH/Benchmark -b range_search -s 1 -i 50 -d --seed 60000
echo
$DIR_PATH/Benchmark -b range_search -s 1 -i 50 --seed 75000