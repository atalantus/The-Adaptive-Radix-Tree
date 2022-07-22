#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>
#include <string>
#include <chrono>
#include "benchmark.h"
#include "data_structures/data_structures.h"
#include "util.h"

constexpr char kUsageMsg[] = "usage: %s [-h] -b benchmark -s size [-i number_iterations] [-d] [--skip structure_list] [-v] \n";
constexpr char kHelpMsg[] = "This program benchmarks different indexing structures using 32 bit unsigned integers. "
                            "For the specified benchmark and size the benchmark is run number_iterations times for each "
                            "index structure and the min, max and average times are outputted.\n\n"
                            "usage: %s [-h] -b benchmark -s size [-i number_iterations] [-d] [--skip structure_list] [-v] \n\n"
                            "\nThe parameters in detail:\n"
                            "\t-h\t\t\t\t: Shows how to use the program (this text).\n"
                            "\t-b <insert/search/range_search>\t: Specifies the benchmark to run. You can either benchmark insertion, searching or searching in range.\n"
                            "\t-s <1/2/3>\t\t\t: Specifies the benchmark size. Options are 1 with 65 thousand integers, 2 with 16 million integers and 3 with 256 million integers.\n"
                            "\t-i <number>\t\t\t: Specifies the number of iterations the benchmark is run. Default value is %u. Should be an integer between 1 and 10000 (inclusive).\n"
                            "\t-d\t\t\t\t: Use a dense (from 0 up to number of elements - 1) set of integers as keys. Otherwise a sparse (uniform random 32 bit integer) set will be used.\n"
                            "\t--skip <structure_list>\t\t\t: Specifies index structures to be skipped during this benchmark. Given as comma separated list of names (ART, Trie, M-Trie, H-Trie, Sorted List, Hash-Table, RB-Tree).\n"
                            "\t-v\t\t\t\t: Enable verbose logging.\n";

/**
 * List of Index Structures each with a name, the number of tabs after the name (used for printing benchmark table)
 * and it's own Benchmark object.
 */
const std::vector<std::tuple<std::string, uint8_t, Benchmark*>> kIndexStructures{
    // Do Sorted List first as it's results will be used to test the other structures
    {"ART", 2, new ArtBenchmark()},
    {"Trie", 2, new TrieBenchmark()},
    {"M-Trie", 2, new MTrieBenchmark()},
    //{"H-Trie", 2, new HTrieBenchmark()},
    {"Sorted List", 1, new SortedListBenchmark()},
    {"Hash-Table", 1, new HashTableBenchmark()},
    {"RB-Tree", 2, new RbTreeBenchmark()}
};

constexpr uint32_t kDefaultIterations{1};

enum class BenchmarkTypes
{
    kInsert,
    kSearch,
    kRangeSearch
};

/**
 * Benchmark Parameters.
 */
BenchmarkTypes benchmark;
uint32_t size = 0;
uint32_t number_elements = 0;
uint32_t iterations{kDefaultIterations};
std::set<std::string> skip;
bool dense = false;
bool verbose = false;

void GenerateRandomNumbers(std::vector<uint32_t>& numbers, std::vector<uint32_t>& search_numbers)
{
    std::random_device rnd;
    std::mt19937_64 eng(rnd());
    //eng.seed(1337);
    const uint32_t s = dense ? number_elements - 1 : 4'294'967'295;
    std::uniform_int_distribution<uint32_t> numbers_distr(0, s);
    std::uniform_int_distribution<uint32_t> search_numbers_distr(0, number_elements - 1);

    numbers.reserve(number_elements);

    if (verbose)
        std::cout << "\nAllocating Memory for numbers..." << std::endl;

    for (uint32_t i = 0; i < number_elements; ++i)
    {
        numbers.push_back(numbers_distr(eng));
    }

    if (benchmark == BenchmarkTypes::kSearch)
    {
        if (verbose)
            std::cout << "Allocating Memory for search_numbers..." << std::endl;

        search_numbers.reserve(number_elements);

        for (uint32_t i = 0; i < number_elements; ++i)
            search_numbers.push_back(numbers[search_numbers_distr(eng)]);
    }
    else if (benchmark == BenchmarkTypes::kRangeSearch)
    {
        if (verbose)
            std::cout << "Allocating Memory for search_numbers..." << std::endl;

        search_numbers.reserve(2ULL * number_elements);

        for (uint64_t i = 0; i < 2ULL * number_elements; i += 2)
        {
            uint32_t n1 = numbers_distr(eng);
            uint32_t n2 = numbers_distr(eng);
            search_numbers.push_back(std::min(n1, n2));
            search_numbers.push_back(std::max(n1, n2));
        }
    }
}

void AggregateBenchmarkResults(auto& total, const std::vector<double>& results, const uint32_t iteration)
{
    for (uint32_t i = 0; i < kIndexStructures.size(); ++i)
    {
        // first iteration
        if (iteration == 0)
        {
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

auto RunBenchmarkIteration()
{
    std::vector<double> structure_times(kIndexStructures.size());

    std::vector<uint32_t> numbers;
    std::vector<uint32_t> search_numbers;
    std::vector<bool> expected_search;
    std::vector<std::vector<uint32_t>> expected_range_search;

    GenerateRandomNumbers(numbers, search_numbers);

    if (verbose)
        std::cout << "Finished allocating Memory for Numbers." << std::endl;

    /**
     * Benchmark Index Structures
     */

    for (uint32_t i = 0; i < kIndexStructures.size(); ++i)
    {
        const auto& [name, _, structure] = kIndexStructures[i];

        if (skip.contains(name)) continue;

        if (verbose)
            std::cout << "\nAllocating " << name << "..." << std::endl;

        structure->InitializeStructure();

        structure->Insert(numbers);

        if (benchmark == BenchmarkTypes::kSearch)
            structure->Search(search_numbers);
        else if (benchmark == BenchmarkTypes::kRangeSearch)
            structure->RangeSearch(search_numbers);

        if (verbose)
            std::cout << "Finished " << name << ". Deleting..." << std::endl;

        structure->DeleteStructure();
    }

    return structure_times;
}

void RunBenchmark()
{
    auto benchmark_to_string = []
    {
        switch (benchmark)
        {
            case BenchmarkTypes::kInsert:
                return "insert";
            case BenchmarkTypes::kSearch:
                return "search";
            case BenchmarkTypes::kRangeSearch:
                return "range_search";
        }

        __unreachable();
    };

    switch (size)
    {
        case 1:
            number_elements = 65'000;
            break;
        case 2:
            number_elements = 16'000'000;
            break;
        case 3:
            number_elements = 250;
    }

    std::cout << "Starting '" << benchmark_to_string() << "' benchmark with size '" << size << "' (" << number_elements
        << " keys), '" << iterations << "' iterations and '" << (dense ? "dense" : "sparse") << "' keys.\n" <<
        std::endl;

    // Run Benchmarks
    for (uint32_t i = 0; i < iterations; ++i)
    {
        if (verbose)
            std::cout << "Running iteration " << (i + 1) << "/" << iterations << "..." << std::endl;
        RunBenchmarkIteration();
    }

    std::cout << "Finished '" << benchmark_to_string() << "' benchmark with size '" << size << "' (" << number_elements << " keys), '" <<
        iterations << "' iterations and '" << (dense ? "dense" : "sparse") << "' keys.\n" << std::endl;
}

int main(int argc, char* argv[])
{
    if constexpr (sizeof(intptr_t) != 8)
    {
        std::cerr << "Please make sure to compile this program for 64 bit architecture!" << std::endl;
        return EXIT_FAILURE;
    }

    size = 1;
    iterations = 1;
    dense = false;
    verbose = true;

    /**
     * Run Search Test.
     */
    benchmark = BenchmarkTypes::kSearch;
    RunBenchmark();

    /**
     * Run Mini RangeSearch Test.
     */
    benchmark = BenchmarkTypes::kRangeSearch;
    skip.insert("H-Trie");
    skip.insert("Hash-Table");
    size = 3;
    RunBenchmark();

    /**
     * Run RangeSearch Test.
     */
    benchmark = BenchmarkTypes::kRangeSearch;
    size = 1;
    RunBenchmark();

    for (uint32_t i = 0; i < kIndexStructures.size(); ++i)
    {
        const auto& [name, spacing, structure] = kIndexStructures[i];

        // Delete Structure Benchmark
        delete structure;
    }

    return EXIT_SUCCESS;
}
