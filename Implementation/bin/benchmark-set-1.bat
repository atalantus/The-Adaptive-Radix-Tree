:: Execute a set of different benchmarks for size 1

@ECHO off
SET DIR_PATH=../out/build/msvc-x64-release

:: Memory
START /WAIT /B %DIR_PATH%/Memory-Benchmark.exe -b insert -s 1 -i 1000 -d --seed 1 %*
ECHO.
START /WAIT /B %DIR_PATH%/Memory-Benchmark.exe -b insert -s 1 -i 1000 --seed 15000 %*
ECHO.

:: Insert
START /WAIT /B %DIR_PATH%/Benchmark.exe -b insert -s 1 -i 1000 -d --seed 1 %*
ECHO.
START /WAIT /B %DIR_PATH%/Benchmark.exe -b insert -s 1 -i 1000 --seed 15000 %*
ECHO.

:: Search
START /WAIT /B %DIR_PATH%/Benchmark.exe -b search -s 1 -i 1000 -d --seed 30000 %*
ECHO.
START /WAIT /B %DIR_PATH%/Benchmark.exe -b search -s 1 -i 1000 --seed 45000 %*
ECHO.

:: Range Search
START /WAIT /B %DIR_PATH%/Benchmark.exe -b range_search -s 1 -i 10 -d --seed 60000 %*
ECHO.
START /WAIT /B %DIR_PATH%/Benchmark.exe -b range_search -s 1 -i 3 --seed 75000 %*