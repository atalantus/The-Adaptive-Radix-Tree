#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>
#include <string>
#include <chrono>
#include "benchmark.h"
#include "data_structures.h"

/**
 * List of Index Structures each with a name, the number of tabs after the name (used for printing benchmark table),
 * a boolean representing if the structure is a trie (used for key transformation) and it's own Benchmark object.
 */
const std::vector<std::tuple<std::string, uint8_t, Benchmark*>> kIndexStructures{
    // Do Sorted List first as it's results will be used to test the other structures
    {"Sorted List", 1, new SortedListBenchmark()},
    {"ART", 2, new ArtBenchmark()},
    {"ART (Virt)", 1, new ArtVirtBenchmark()},
    {"ART (CRTP)", 1, new ArtCRTPBenchmark()},
    {"ART (Leis)", 1, new ArtLeisBenchmark()},
    //{"Trie", 2, new TrieBenchmark()},
    //{"M-Trie", 2, new MTrieBenchmark()},
    //{"H-Trie", 2, new HTrieBenchmark()},
    //{"Hash-Table", 1, new HashTableBenchmark()},
    //{"RB-Tree", 2, new RbTreeBenchmark()}
};

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
uint32_t iterations = 0;
std::set<std::string> skip;
bool dense = false;
bool verbose = false;

auto GenerateRandomNumbers(std::vector<uint32_t>& numbers, std::vector<uint32_t>& search_numbers)
{
    std::random_device rnd;
    const auto seed = rnd();
    std::mt19937_64 eng(seed);
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
        {
            // ensure that at least half of all lookups will be positive
            if (i < number_elements / 2)
                search_numbers.push_back(numbers[search_numbers_distr(eng)]);
            else
                search_numbers.push_back(numbers_distr(eng));
        }
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

    return seed;
}

auto RunBenchmarkIteration()
{
    std::vector<uint32_t> numbers;
    std::vector<uint32_t> search_numbers;
    std::vector<bool> expected_search;
    std::vector<std::vector<uint32_t>> expected_range_search;

    const auto seed = GenerateRandomNumbers(numbers, search_numbers);

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
            structure->Search(search_numbers, expected_search);
        else if (benchmark == BenchmarkTypes::kRangeSearch)
            structure->RangeSearch(search_numbers, expected_range_search);

        if (verbose)
            std::cout << "Finished " << name << ". Deleting..." << std::endl;

        structure->DeleteStructure();
    }

    return seed;
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
            number_elements = 10'000;
    }

    std::cout << "Starting '" << benchmark_to_string() << "' benchmark with size '" << size << "' (" << number_elements
        << " keys), '" << iterations << "' iterations and '" << (dense ? "dense" : "sparse") << "' keys.\n" <<
        std::endl;

    // Run Benchmarks
    for (uint32_t i = 0; i < iterations; ++i)
    {
        const auto seed = RunBenchmarkIteration();

        std::cout << "Finished iteration " << (i + 1) << "/" << iterations << " with seed " << seed << std::endl;
    }

    std::cout << "\nFinished '" << benchmark_to_string() << "' benchmark with size '" << size << "' (" << number_elements << " keys), '" <<
        iterations << "' iterations and '" << (dense ? "dense" : "sparse") << "' keys.\n" << std::endl;
}

int main(int argc, char* argv[])
{
    if constexpr (sizeof(intptr_t) != 8)
    {
        std::cerr << "Please make sure to compile this program for 64 bit architecture!" << std::endl;
        return EXIT_FAILURE;
    }

    iterations = 2;
    verbose = false;

    /**
     * Run Search Tests.
     */
    benchmark = BenchmarkTypes::kSearch;

    // Search Test 1
    size = 1;
    dense = false;
    RunBenchmark();

    // Search Test 2
    size = 2;
    dense = true;
    RunBenchmark();

    /**
     * Run RangeSearch Tests.
     */
    benchmark = BenchmarkTypes::kRangeSearch;
    skip.insert("ART (Leis)");
    skip.insert("H-Trie");
    skip.insert("Hash-Table");

    // RangeSearch Test 1
    size = 1;
    dense = false;
    RunBenchmark();

    // RangeSearch Test 2
    size = 1;
    dense = true;
    RunBenchmark();

    for (uint32_t i = 0; i < kIndexStructures.size(); ++i)
    {
        const auto& [name, spacing, structure] = kIndexStructures[i];

        // Delete Structure Benchmark
        delete structure;
    }

    return EXIT_SUCCESS;
}