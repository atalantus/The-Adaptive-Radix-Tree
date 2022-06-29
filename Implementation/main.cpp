#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include "clock.h"
#include "benchmark.h"
#include "data_structures/data_structures.h"
#include "util.h"

constexpr char kUsageMsg[] = "usage: %s [-h] -b benchmark -s size [-i number_iterations]\n";
constexpr char kHelpMsg[] = "This program benchmarks different indexing structures using 32 bit unsigned integers. "
    "For the specified benchmark and size the benchmark is run number_iterations times for each "
    "index structure and the min, max and average times are outputted.\n\n"
    "usage: %s [-h] -b benchmark -s size [-i number_iterations]\n"
    "\nThe parameters in detail:\n"
    "\t-h\t\t\t\t: Shows how to use the program (this text).\n"
    "\t-b <insert/search/range_search>\t: Specifies the benchmark to run. You can either benchmark insertion, searching or searching in range.\n"
    "\t-s <1/2>\t\t\t: Specifies the benchmark size. Options are 1 with 65 thousand integers and 2 with 16 million integers.\n"
    "\t-i <number>\t\t\t: Specifies the number of iterations the benchmark is run. Default value is %u. Should be an integer between 1 and 10000 (inclusive).\n";

/**
 * List of Index Structures each with a name, the number of tabs after the name (used for printing benchmark table)
 * and it's own Benchmark object.
 */
const std::vector<std::tuple<std::string, uint8_t, Benchmark*>> kIndexStructures{
    {"ART", 2, new ArtBenchmark()},
    {"Trie", 2, new TrieBenchmark()},
    {"CTrie", 2, new CTrieBenchmark()},
    {"Sorted List", 1, new SortedListBenchmark()},
    {"Hash Table", 1, new HashTableBenchmark()},
    {"Red-black Tree", 1, new RbTreeBenchmark()}
};

constexpr uint32_t kDefaultIterations{1};

enum class BenchmarkTypes
{
    kInsert,
    kSearch,
    kRangeSearch
};

void GenerateRandomNumbers(uint32_t*& numbers, uint32_t*& search_numbers,
                           const BenchmarkTypes benchmark, const uint32_t number_elements)
{
    std::random_device rnd;
    std::mt19937_64 eng(rnd());
    std::uniform_int_distribution<uint32_t> numbers_distr;
    std::uniform_int_distribution<uint32_t> search_numbers_distr(0, number_elements);

    numbers = new uint32_t[number_elements];
    search_numbers = nullptr;

    std::cout << "\nAllocating Memory for numbers..." << std::endl;

    for (uint32_t i = 0; i < number_elements; ++i)
        numbers[i] = numbers_distr(eng);

    if (benchmark == BenchmarkTypes::kSearch)
    {
        std::cout << "Allocating Memory for search_numbers..." << std::endl;

        search_numbers = new uint32_t[number_elements];

        for (uint32_t i = 0; i < number_elements; ++i)
            search_numbers[i] = numbers[search_numbers_distr(eng)];
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

auto RunBenchmarkIteration(const BenchmarkTypes benchmark, const uint32_t size)
{
    std::vector<double> structure_times(kIndexStructures.size());

    uint32_t number_elements = 0;

    switch (size)
    {
        case 1:
            number_elements = 65'000;
            break;
        case 2:
            number_elements = 16'000'000;
            break;
        case 3:
            number_elements = 256'000'000;
            break;
    }

    uint32_t* numbers = nullptr;
    uint32_t* search_numbers = nullptr;

    GenerateRandomNumbers(numbers, search_numbers, benchmark, number_elements);

    std::cout << "Finished allocating Memory for Numbers." << std::endl;

    Clock clock;
    double time{0.0};

    /**
     * Benchmark Index Structures
     */

    for (uint32_t i = 0; i < kIndexStructures.size(); ++i)
    {
        const auto& [name, _, structure] = kIndexStructures[i];

        std::cout << "\nAllocating " << name << "..." << std::endl;

        structure->InitializeStructure();

        clock.Start();
        structure->Insert(numbers, number_elements);
        time = clock.Stop();

        if (benchmark == BenchmarkTypes::kSearch)
        {
            clock.Start();
            structure->Search(search_numbers, number_elements);
            time = clock.Stop();
        }

        std::cout << "Finished " << name << ". Deleting..." << std::endl;

        structure_times[i] = time;
        structure->DeleteStructure();
    }

    /**
     * Free Memory
     */
    std::cout << "\nDeleting Memory...\n" << std::endl;

    delete[] numbers;

    switch (benchmark)
    {
        case BenchmarkTypes::kSearch:
            delete[] search_numbers;
            break;
        case BenchmarkTypes::kRangeSearch:
            break;
    }

    return structure_times;
}

void RunBenchmark(BenchmarkTypes benchmark, uint32_t size, const uint32_t iterations)
{
    auto benchmark_to_string = [benchmark]()
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
    };

    std::cout << "Starting '" << benchmark_to_string() << "' benchmark with size '" << size << "' and '" << iterations
        << "' iterations.\n" << std::endl;

    std::vector<std::tuple<double, double, double, double>> structureTimes(kIndexStructures.size());

    // Run Benchmarks
    for (uint32_t i = 0; i < iterations; ++i)
    {
        std::cout << "Running iteration " << (i + 1) << "/" << iterations << "..." << std::endl;
        AggregateBenchmarkResults(structureTimes, RunBenchmarkIteration(benchmark, size), i);
    }

    std::cout << "Finished '" << benchmark_to_string() << "' benchmark with size '" << size << "' and '" << iterations
        << "' iterations.\n" << std::endl;

    std::cout << "=================================================================" << std::endl;
    std::cout << "\t\t\tBENCHMARK RESULTS\t\t\t" << std::endl;
    std::cout << "=================================================================" << std::endl;

    std::cout << "Index Structure\t|\tMin\t|\tMax\t|\tAvg\t|" << std::endl;
    std::cout << "-----------------------------------------------------------------" << std::endl;

    std::cout.precision(2);

    for (uint32_t i = 0; i < kIndexStructures.size(); ++i)
    {
        auto& times = structureTimes[i];

        std::cout << std::get<0>(kIndexStructures[i]);
        for (uint8_t j = 0; j < std::get<1>(kIndexStructures[i]); ++j)
        {
            std::cout << "\t";
        }
        std::cout << "|\t";

        std::cout << std::fixed
            << std::get<0>(times) << "s\t|\t"
            << std::get<1>(times) << "s\t|\t"
            << std::get<2>(times) << "s\t|\t" << std::endl;

        // Delete Structure Benchmark
        delete std::get<2>(kIndexStructures[i]);
    }
}

int main(int argc, char* argv[])
{
    if (CmdArgExists(argv, argv + argc, "-h"))
    {
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

    const std::string benchmark_str{"search"};
    const std::string size_str{"2"};

    /**
     * Benchmark Parameters.
     */
    BenchmarkTypes benchmark;
    uint32_t size = 0;
    uint32_t iterations{kDefaultIterations};

    if (benchmark_str == "insert")
    {
        benchmark = BenchmarkTypes::kInsert;
    }
    else if (benchmark_str == "search")
    {
        benchmark = BenchmarkTypes::kSearch;
    }
    else if (benchmark_str == "range_search")
    {
        benchmark = BenchmarkTypes::kRangeSearch;
    }
    else
    {
        std::cerr << "Unknown 'benchmark' argument \"" << benchmark_str <<
            R"(". Possible options are "insert", "search" and "range_search".)" << std::endl;
        return EXIT_FAILURE;
    }

    try
    {
        size = std::stoul(size_str);
    }
    catch (std::logic_error&)
    {
        std::cerr << "Invalid 'size' argument \"" << size << R"(". Possible options are "1", "2", "3".)"
            << std::endl;
        return EXIT_FAILURE;
    }

    if (size < 1 || size > 3)
    {
        std::cerr << "Invalid 'size' argument \"" << size << R"(". Possible options are "1", "2", "3".)"
            << std::endl;
        return EXIT_FAILURE;
    }

    if (iterations_arg != nullptr)
    {
        const std::string iterations_str{iterations_arg};

        try
        {
            iterations = std::stoul(iterations_str);
        }
        catch (std::logic_error&)
        {
            std::cerr << "Invalid 'number_iterations' argument \"" << iterations_str
                << "\". Expected integer between 1 and 10000 (inclusive)." << std::endl;
            return EXIT_FAILURE;
        }

        if (iterations < 1 || iterations > 10000)
        {
            std::cerr << "Invalid 'number_iterations' argument \"" << iterations_str
                << "\". Expected integer between 1 and 10000 (inclusive)." << std::endl;
            return EXIT_FAILURE;
        }
    }

    /**
     * Run Benchmark.
     */
    RunBenchmark(benchmark, size, iterations);

    std::cout << "\nPress any key to exit . . .";
    std::getchar();

    return EXIT_SUCCESS;
}
