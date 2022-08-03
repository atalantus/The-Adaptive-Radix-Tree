#!/bin/sh

# Execute a set of different benchmarks for size 2

DIR_PATH="../out/build/gcc-x64-release"

# Memory
$DIR_PATH/Memory-Benchmark -b insert -s 2 -i 50 -d --seed 1 $@
echo
$DIR_PATH/Memory-Benchmark -b insert -s 2 -i 50 --skip Trie --seed 15000 $@
echo

# Insert
$DIR_PATH/Benchmark -b insert -s 2 -i 50 -d --seed 1 $@
echo
$DIR_PATH/Benchmark -b insert -s 2 -i 50 --skip Trie --seed 15000 $@
echo

# Search
$DIR_PATH/Benchmark -b search -s 2 -i 50 -d --seed 30000 $@
echo
$DIR_PATH/Benchmark -b search -s 2 -i 50 --skip Trie --seed 45000 $@
echo

# Range Search
$DIR_PATH/Benchmark -b range_search -s 2 -i 10 -d --seed 60000 $@
echo
$DIR_PATH/Benchmark -b range_search -s 2 -i 3 --skip Trie --seed 75000 $@