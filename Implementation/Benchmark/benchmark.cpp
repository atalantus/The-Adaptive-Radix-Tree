#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include "trie.h"

const char USAGE_MSG[] = "usage: %s [-h] -b benchmark -s size [-i number_iterations]\n";
const char HELP_MSG[] = "This program benchmarks different indexing structures using 32 bit unsigned integers. "
                        "For the specified benchmark and size the benchmark is run number_iterations times for each "
                        "index structure and the min, max, average and median times are outputted.\n\n"
                        "usage: %s [-h] -b benchmark -s size [-i number_iterations]\n"
                        "\nThe parameters in detail:\n"
                        "\t-h\t\t\t\t: Shows how to use the program (this text).\n"
                        "\t-b <insert/search/range_search>\t: Specifies the benchmark to run. You can either benchmark insertion, searching or searching in range.\n"
                        "\t-s <1/2/3>\t\t\t: Specifies the benchmark size. Options are 1 with 65 thousand integers, 2 with 16 million integers and 3 with 256 million integers.\n"
                        "\t-i <number>\t\t\t: Specifies the number of iterations the benchmark is run. Default value is %u. Should be an integer between 1 and 10000 (inclusive).\n";

const uint32_t DEFAULT_ITERATIONS = 10;

enum class Benchmark {
    insert, search, range_search
};

void runBenchmark(Benchmark benchmark, uint16_t size, uint32_t iterations) {
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

    std::cout << "Running '" << benchmarkToString() << "' benchmark with size '" << size << "' and '" << iterations
              << "' iterations..." << std::endl;

    // TODO: Run benchmark

    std::cout << "Finished running '" << benchmarkToString() << "' benchmark." << std::endl;
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
    uint16_t size;
    uint32_t iterations{DEFAULT_ITERATIONS};

    if (benchmarkStr == "insert") {
        benchmark = Benchmark::insert;
    } else if (benchmarkStr == "search") {
        benchmark = Benchmark::search;
    } else if (benchmarkStr == "range_search") {
        benchmark = Benchmark::range_search;
    } else {
        std::cerr << "Unknown 'benchmark' argument \"" << benchmarkStr
                  << "\". Possible options are \"insert\", \"search\" and \"range_search\"." << std::endl;
        return EXIT_FAILURE;
    }

    try {
        size = std::stoul(sizeStr);
    } catch (std::logic_error&) {
        std::cerr << "Invalid 'size' argument \"" << size << "\". Possible options are \"1\", \"2\", \"3\"."
                  << std::endl;
        return EXIT_FAILURE;
    }

    if (size < 1 || size > 3) {
        std::cerr << "Invalid 'size' argument \"" << size << "\". Possible options are \"1\", \"2\", \"3\"."
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
