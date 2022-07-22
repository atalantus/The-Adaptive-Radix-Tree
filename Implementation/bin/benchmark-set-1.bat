:: Execute a set of different benchmarks for size 1

@ECHO off
SET DIR_PATH=../out/build/msvc-x64-release

:: Insert
START /WAIT /B %DIR_PATH%/Benchmark.exe -b insert -s 1 -i 10 -d
ECHO.
START /WAIT /B %DIR_PATH%/Benchmark.exe -b insert -s 1 -i 10
ECHO.

:: Search
START /WAIT /B %DIR_PATH%/Benchmark.exe -b search -s 1 -i 10 -d
ECHO.
START /WAIT /B %DIR_PATH%/Benchmark.exe -b search -s 1 -i 10
ECHO.

:: Range Search
START /WAIT /B %DIR_PATH%/Benchmark.exe -b range_search -s 1 -i 5 -d
ECHO.
START /WAIT /B %DIR_PATH%/Benchmark.exe -b range_search -s 1 -i 5