:: Execute a set of different benchmarks for size 2

@ECHO off
SET DIR_PATH=../out/build/msvc-x64-release

:: Insert
START /WAIT /B ./%DIR_PATH%/Benchmark.exe -b insert -s 2 -i 5 -d
ECHO.
START /WAIT /B ./%DIR_PATH%/Benchmark.exe -b insert -s 2 -i 5 --skip Trie
ECHO.

:: Search
START /WAIT /B ./%DIR_PATH%/Benchmark.exe -b search -s 2 -i 5 -d
ECHO.
START /WAIT /B ./%DIR_PATH%/Benchmark.exe -b search -s 2 -i 5 --skip Trie
ECHO.

:: Range Search
START /WAIT /B ./%DIR_PATH%/Benchmark.exe -b range_search -s 2 -i 3 -d
ECHO.
START /WAIT /B ./%DIR_PATH%/Benchmark.exe -b range_search -s 2 -i 3 --skip Trie