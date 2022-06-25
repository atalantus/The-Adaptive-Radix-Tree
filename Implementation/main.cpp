#pragma once

#include <algorithm>
#include <iostream>
#include <random>
#include <functional>
#include <string>
#include <chrono>
#include "data_structures/data_structures.h"
#include "benchmark.h"

const char USAGE_MSG[] = "usage: %s [-h] -b benchmark -s size [-i number_iterations]\n";
const char HELP_MSG[] = "This program benchmarks different indexing structures using 32 bit unsigned integers. "
                        "For the specified benchmark and size the benchmark is run number_iterations times for each "
                        "index structure and the min, max and average times are outputted.\n\n"
                        "usage: %s [-h] -b benchmark -s size [-i number_iterations]\n"
                        "\nThe parameters in detail:\n"
                        "\t-h\t\t\t\t: Shows how to use the program (this text).\n"
                        "\t-b <insert/search/range_search>\t: Specifies the benchmark to run. You can either benchmark insertion, searching or searching in range.\n"
                        "\t-s <1/2/3>\t\t\t: Specifies the benchmark size. Options are 1 with 65 thousand integers, 2 with 16 million integers and 3 with 256 million integers.\n"
                        "\t-i <number>\t\t\t: Specifies the number of iterations the benchmark is run. Default value is %u. Should be an integer between 1 and 10000 (inclusive).\n";

const std::vector<std::string> INDEX_STRUCTURES{"ART", "Trie"};

const uint32_t DEFAULT_ITERATIONS{10};

const uint32_t STRUCTURES_SIZE{INDEX_STRUCTURES.size()};

enum class Benchmark {
    insert, search, range_search
};

void aggregateBenchmarkResults(auto& total, const std::vector<double>& results, int iteration) {
    for (int i = 0; i < STRUCTURES_SIZE; ++i) {
        // first iteration
        if (iteration == 0) {
            total[i] = {results[i], results[i], results[i], results[i]};
            continue;
        }

        // Min
        if (std::get<0>(total[i]) > results[i])
            std::get<0>(total[i]) = results[i];

        // Max
        if (std::get<1>(total[i]) < results[i])
            std::get<1>(total[i]) = results[i];

        // Avg
        std::get<2>(total[i]) = (std::get<3>(total[i]) + results[i]) / (iteration + 1);

        // Sum
        std::get<3>(total[i]) += results[i];
    }
}

double testIndexStructure(const std::function<void(std::vector<uint32_t>)>& benchmark,
                          const std::vector<uint32_t>& numbers) {
    auto t1 = std::chrono::high_resolution_clock::now();

    benchmark(numbers);

    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> dur{t2 - t1};

    return dur.count() / 1000.0;
}

auto runBenchmarkIteration(Benchmark benchmark, uint32_t size) {
    std::vector<double> structureTimes(STRUCTURES_SIZE);

    /*
     * Pre-Generate Random Numbers
     */
    std::random_device rnd;
    std::mt19937_64 eng(rnd());
    std::uniform_int_distribution<uint32_t> distr;

    std::vector<uint32_t> numbers(size);

    for (auto& n: numbers) {
        n = distr(eng);
    }

    switch (benchmark) {
        case Benchmark::insert:
            structureTimes[0] = testIndexStructure({art_insert}, numbers);
            structureTimes[1] = testIndexStructure({trie_insert}, numbers);
            break;
        case Benchmark::search:
            break;
        case Benchmark::range_search:
            break;
    }

    return structureTimes;
}

void runBenchmark(Benchmark benchmark, uint32_t size, uint32_t iterations) {
    auto benchmarkToString = [benchmark]() {
        switch (benchmark) {
            case Benchmark::insert:
                return "insert";
            case Benchmark::search:
                return "search";
            case Benchmark::range_search:
                return "range_search";
        }
    };

    std::cout << "Starting '" << benchmarkToString() << "' benchmark with size '" << size << "' and '" << iterations
              << "' iterations.\n" << std::endl;

    switch (size) {
        case 1:
            size = 65'000;
            break;
        case 2:
            size = 16'000'000;
            break;
        case 3:
            size = 256'000'000;
            break;
    }

    std::vector<std::tuple<double, double, double, double>> structureTimes(STRUCTURES_SIZE);

    // Run Benchmarks
    for (int i = 0; i < iterations; ++i) {
        aggregateBenchmarkResults(structureTimes, runBenchmarkIteration(benchmark, size), i);
    }


    for (int i = 0; i < iterations; ++i) {
        std::cout << "Running iteration " << (i + 1) << "/" << iterations << "..." << std::endl;
    }

    std::cout << "\nFinished running '" << benchmarkToString() << "' benchmark.\n" << std::endl;

    std::cout << "=================================================================" << std::endl;
    std::cout << "\t\t\tBENCHMARK RESULTS\t\t\t" << std::endl;
    std::cout << "=================================================================" << std::endl;

    std::cout << "Index Structure\t|\tMin\t|\tMax\t|\tAvg\t|" << std::endl;
    std::cout << "-----------------------------------------------------------------" << std::endl;

    std::cout.precision(2);

    for (int i = 0; i < STRUCTURES_SIZE; ++i) {
        auto times = structureTimes[i];

        std::cout << std::fixed << INDEX_STRUCTURES[i] << "\t\t|\t"
                  << std::get<0>(times) << "s\t|\t"
                  << std::get<1>(times) << "s\t|\t"
                  << std::get<2>(times) << "s\t|\t" << std::endl;
    }
}

char* getCmdArg(char** begin, char** end, const std::string& arg) {
    char** itr = std::find(begin, end, arg);
    if (itr != end && ++itr != end)
        return *itr;
    return nullptr;
}

bool cmdArgExists(char** begin, char** end, const std::string& arg) {
    return std::find(begin, end, arg) != end;
}

int main(int argc, char* argv[]) {
    if (cmdArgExists(argv, argv + argc, "-h")) {
        printf(HELP_MSG, argv[0], DEFAULT_ITERATIONS);
        return EXIT_SUCCESS;
    }

    if (argc < 5) {
        fprintf(stderr, USAGE_MSG, argv[0]);
        return EXIT_FAILURE;
    }

    char* benchmarkArg = getCmdArg(argv, argv + argc, "-b");
    char* sizeArg = getCmdArg(argv, argv + argc, "-s");
    char* iterationsArg = getCmdArg(argv, argv + argc, "-i");

    if (benchmarkArg == nullptr || sizeArg == nullptr) {
        fprintf(stderr, USAGE_MSG, argv[0]);
        return EXIT_FAILURE;
    }

    std::string benchmarkStr{benchmarkArg};
    std::string sizeStr{sizeArg};

    /**
     * Benchmark Parameters.
     */
    Benchmark benchmark;
    uint32_t size;
    uint32_t iterations{DEFAULT_ITERATIONS};

    if (benchmarkStr == "insert") {
        benchmark = Benchmark::insert;
    } else if (benchmarkStr == "search") {
        benchmark = Benchmark::search;
    } else if (benchmarkStr == "range_search") {
        benchmark = Benchmark::range_search;
    } else {
        std::cerr << "Unknown 'benchmark' argument \"" << benchmarkStr
                  << R"(". Possible options are "insert", "search" and "range_search".)" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        size = std::stoul(sizeStr);
    } catch (std::logic_error&) {
        std::cerr << "Invalid 'size' argument \"" << size << R"(". Possible options are "1", "2", "3".)"
                  << std::endl;
        return EXIT_FAILURE;
    }

    if (size < 1 || size > 3) {
        std::cerr << "Invalid 'size' argument \"" << size << R"(". Possible options are "1", "2", "3".)"
                  << std::endl;
        return EXIT_FAILURE;
    }

    if (iterationsArg != nullptr) {
        std::string iterationsStr{iterationsArg};

        try {
            iterations = std::stoul(iterationsStr);
        } catch (std::logic_error&) {
            std::cerr << "Invalid 'number_iterations' argument \"" << iterationsStr
                      << "\". Expected integer between 1 and 10000 (inclusive)." << std::endl;
            return EXIT_FAILURE;
        }

        if (iterations < 1 || iterations > 10000) {
            std::cerr << "Invalid 'number_iterations' argument \"" << iterationsStr
                      << "\". Expected integer between 1 and 10000 (inclusive)." << std::endl;
            return EXIT_FAILURE;
        }
    }

    /**
     * Run Benchmark.
     */
    runBenchmark(benchmark, size, iterations);

    return EXIT_SUCCESS;
}
