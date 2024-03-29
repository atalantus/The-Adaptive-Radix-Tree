:: Execute a set of different benchmarks for size 2

@ECHO off
SET DIR_PATH=../out/build/msvc-x64-release

:: Memory
START /WAIT /B ./%DIR_PATH%/Memory-Benchmark.exe -b insert -s 2 -i 5 -d --seed 1 %*
ECHO.
START /WAIT /B ./%DIR_PATH%/Memory-Benchmark.exe -b insert -s 2 -i 5 --seed 15000 %*
ECHO.

:: Insert
START /WAIT /B ./%DIR_PATH%/Benchmark.exe -b insert -s 2 -i 20 -d --seed 1 %*
ECHO.
START /WAIT /B ./%DIR_PATH%/Benchmark.exe -b insert -s 2 -i 20 --skip Trie --seed 15000 %*
ECHO.

:: Search
START /WAIT /B ./%DIR_PATH%/Benchmark.exe -b search -s 2 -i 20 -d --seed 30000 %*
ECHO.
START /WAIT /B ./%DIR_PATH%/Benchmark.exe -b search -s 2 -i 20 --skip Trie --seed 45000 %*
ECHO.

:: Range Search
START /WAIT /B ./%DIR_PATH%/Benchmark.exe -b range_search -s 2 -i 5 -d --seed 60000 %*
ECHO.
START /WAIT /B ./%DIR_PATH%/Benchmark.exe -b range_search -s 2 -i 3 --skip Trie --seed 75000 %*