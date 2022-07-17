#!/bin/sh

# Execute a set of different benchmarks for size 2

DIR_PATH="./out/build/gcc-x64-release"

# Insert
$DIR_PATH/Benchmark -b insert -s 2 -i 5 -d
echo
$DIR_PATH/Benchmark -b insert -s 2 -i 5 --skip Trie
echo

# Search
$DIR_PATH/Benchmark -b search -s 2 -i 5 -d
echo
$DIR_PATH/Benchmark -b search -s 2 -i 5 --skip Trie
echo

# Range Search
$DIR_PATH/Benchmark -b range_search -s 2 -i 3 -d
echo
$DIR_PATH/Benchmark -b range_search -s 2 -i 3 --skip Trie