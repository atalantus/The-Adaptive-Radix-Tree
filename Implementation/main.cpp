#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include "clock.h"
#include "benchmark.h"
#include "data_structures/data_structures.h"

constexpr char kUsageMsg[] = "usage: %s [-h] -b benchmark -s size [-i number_iterations]\n";
constexpr char kHelpMsg[] = "This program benchmarks different indexing structures using 32 bit unsigned integers. "
                            "For the specified benchmark and size the benchmark is run number_iterations times for each "
                            "index structure and the min, max and average times are outputted.\n\n"
                            "usage: %s [-h] -b benchmark -s size [-i number_iterations]\n"
                            "\nThe parameters in detail:\n"
                            "\t-h\t\t\t\t: Shows how to use the program (this text).\n"
                            "\t-b <insert/search/range_search>\t: Specifies the benchmark to run. You can either benchmark insertion, searching or searching in range.\n"
                            "\t-s <1/2/3>\t\t\t: Specifies the benchmark size. Options are 1 with 65 thousand integers, 2 with 16 million integers and 3 with 256 million integers.\n"
                            "\t-i <number>\t\t\t: Specifies the number of iterations the benchmark is run. Default value is %u. Should be an integer between 1 and 10000 (inclusive).\n";

const std::vector<std::string> kIndexStructures{"ART", "Trie"};

constexpr uint32_t kDefaultIterations{10};

enum class Benchmark {
    kInsert,
    kSearch,
    kRangeSearch
};

void GenerateRandomNumbers(uint32_t* numbers, uint32_t* search_numbers,
                           const Benchmark benchmark, const uint32_t size) {
    std::random_device rnd;
    std::mt19937_64 eng(rnd());
    const std::uniform_int_distribution<uint32_t> numbers_distr;
    const std::uniform_int_distribution<uint32_t> search_numbers_distr(0, size);

    numbers = new uint32_t[size];
    search_numbers = nullptr;

    for (uint32_t i = 0; i < size; ++i)
        numbers[i] = numbers_distr(eng);

    if (benchmark == Benchmark::kSearch) {
        search_numbers = new uint32_t[size];

        for (uint32_t i = 0; i < size; ++i)
            search_numbers[i] = numbers[search_numbers_distr(eng)];
    }
}

void AggregateBenchmarkResults(auto& total, const std::vector<double>& results, const uint32_t iteration) {
    for (uint32_t i = 0; i < kIndexStructures.size(); ++i) {
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

auto RunBenchmarkIteration(const Benchmark benchmark, const uint32_t size) {
    std::vector<double> structure_times(kIndexStructures.size());

    uint32_t* numbers = nullptr;
    uint32_t* search_numbers = nullptr;

    GenerateRandomNumbers(numbers, search_numbers, benchmark, size);

    Clock clock;
    double time{0.0};

    /**
     * ART Benchmark
     */
    const auto art = new art::Art();

    clock.Start();
    ArtInsert(art, numbers, size);
    time = clock.Stop();

    if (benchmark == Benchmark::kSearch) {
        clock.Start();
        ArtSearch(art, search_numbers, size);
        time = clock.Stop();
    } else if (benchmark == Benchmark::kRangeSearch) {
    }

    structure_times[0] = time;
    delete art;

    /**
     * Trie Benchmark
     */
    const auto trie = new trie::Trie();

    clock.Start();
    TrieInsert(trie, numbers, size);
    time = clock.Stop();

    if (benchmark == Benchmark::kSearch) {
        clock.Start();
        TrieSearch(trie, search_numbers, size);
        time = clock.Stop();
    } else if (benchmark == Benchmark::kRangeSearch) {
    }

    structure_times[1] = time;
    delete trie;

    /**
     * Free Memory
     */
    delete[] numbers;

    switch (benchmark) {
        case Benchmark::kSearch:
            delete[] search_numbers;
            break;
        case Benchmark::kRangeSearch:
            break;
    }

    return structure_times;
}

void RunBenchmark(Benchmark benchmark, uint32_t size, const uint32_t iterations) {
    auto benchmark_to_string = [benchmark]() {
        switch (benchmark) {
            case Benchmark::kInsert:
                return "insert";
            case Benchmark::kSearch:
                return "search";
            case Benchmark::kRangeSearch:
                return "range_search";
        }
    };

    std::cout << "Starting '" << benchmark_to_string() << "' benchmark with size '" << size << "' and '" << iterations
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

    std::vector<std::tuple<double, double, double, double>> structureTimes(kIndexStructures.size());

    // Run Benchmarks
    for (uint32_t i = 0; i < iterations; ++i) {
        std::cout << "Running iteration " << (i + 1) << "/" << iterations << "..." << std::endl;
        AggregateBenchmarkResults(structureTimes, RunBenchmarkIteration(benchmark, size), i);
    }

    std::cout << "\nFinished running '" << benchmark_to_string() << "' benchmark.\n" << std::endl;

    std::cout << "=================================================================" << std::endl;
    std::cout << "\t\t\tBENCHMARK RESULTS\t\t\t" << std::endl;
    std::cout << "=================================================================" << std::endl;

    std::cout << "Index Structure\t|\tMin\t|\tMax\t|\tAvg\t|" << std::endl;
    std::cout << "-----------------------------------------------------------------" << std::endl;

    std::cout.precision(2);

    for (uint32_t i = 0; i < kIndexStructures.size(); ++i) {
        auto& times = structureTimes[i];

        std::cout << std::fixed << kIndexStructures[i] << "\t\t|\t"
                  << std::get<0>(times) << "s\t|\t"
                  << std::get<1>(times) << "s\t|\t"
                  << std::get<2>(times) << "s\t|\t" << std::endl;
    }
}

char* GetCmdArg(char** begin, char** end, const std::string& arg) {
    char** itr = std::find(begin, end, arg);
    if (itr != end && ++itr != end)
        return *itr;
    return nullptr;
}

bool CmdArgExists(char** begin, char** end, const std::string& arg) {
    return std::find(begin, end, arg) != end;
}

int main(int argc, char* argv[]) {
    if (CmdArgExists(argv, argv + argc, "-h")) {
        printf(kHelpMsg, argv[0], kDefaultIterations);
        return EXIT_SUCCESS;
    }

    /*
    if (argc < 5)
    {
        fprintf(stderr, usage_msg, argv[0]);
        return EXIT_FAILURE;
    }
    */

    char* benchmark_arg = GetCmdArg(argv, argv + argc, "-b");
    char* size_arg = GetCmdArg(argv, argv + argc, "-s");
    char* iterations_arg = GetCmdArg(argv, argv + argc, "-i");

    // TODO: Default Values
    /*
    if (benchmark_arg == nullptr || size_arg == nullptr) {
        fprintf(stderr, usage_msg, argv[0]);
        return EXIT_FAILURE;
    }


    const std::string benchmark_str{benchmark_arg};
    const std::string size_str{size_arg};
    */

    const std::string benchmark_str{"insert"};
    const std::string size_str{"2"};

    /**
     * Benchmark Parameters.
     */
    Benchmark benchmark;
    uint32_t size = 0;
    uint32_t iterations{kDefaultIterations};

    if (benchmark_str == "insert") {
        benchmark = Benchmark::kInsert;
    } else if (benchmark_str == "search") {
        benchmark = Benchmark::kSearch;
    } else if (benchmark_str == "range_search") {
        benchmark = Benchmark::kRangeSearch;
    } else {
        std::cerr << "Unknown 'benchmark' argument \"" << benchmark_str
                  << R"(". Possible options are "insert", "search" and "range_search".)" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        size = std::stoul(size_str);
    }
    catch (std::logic_error&) {
        std::cerr << "Invalid 'size' argument \"" << size << R"(". Possible options are "1", "2", "3".)"
                  << std::endl;
        return EXIT_FAILURE;
    }

    if (size < 1 || size > 3) {
        std::cerr << "Invalid 'size' argument \"" << size << R"(". Possible options are "1", "2", "3".)"
                  << std::endl;
        return EXIT_FAILURE;
    }

    if (iterations_arg != nullptr) {
        const std::string iterations_str{iterations_arg};

        try {
            iterations = std::stoul(iterations_str);
        }
        catch (std::logic_error&) {
            std::cerr << "Invalid 'number_iterations' argument \"" << iterations_str
                      << "\". Expected integer between 1 and 10000 (inclusive)." << std::endl;
            return EXIT_FAILURE;
        }

        if (iterations < 1 || iterations > 10000) {
            std::cerr << "Invalid 'number_iterations' argument \"" << iterations_str
                      << "\". Expected integer between 1 and 10000 (inclusive)." << std::endl;
            return EXIT_FAILURE;
        }
    }

    /**
     * Run Benchmark.
     */
    RunBenchmark(benchmark, size, iterations);

    return EXIT_SUCCESS;
}
