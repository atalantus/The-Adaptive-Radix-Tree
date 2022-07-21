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
        {"ART",         2, new ArtBenchmark()},
        {"Trie",        2, new TrieBenchmark()},
        {"M-Trie",      2, new MTrieBenchmark()},
        //{"H-Trie", 2, new HTrieBenchmark()},
        {"Sorted List", 1, new SortedListBenchmark()},
        {"Hash-Table",  1, new HashTableBenchmark()},
        {"RB-Tree",     2, new RbTreeBenchmark()}
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

        auto s1 = std::chrono::system_clock::now();
        structure->Insert(numbers);
        double time = static_cast<double>(std::chrono::duration_cast<
                std::chrono::nanoseconds>(std::chrono::system_clock::now() - s1).count()) / 1e9;

        if (benchmark == BenchmarkTypes::kSearch)
        {
            s1 = std::chrono::system_clock::now();
            structure->Search(search_numbers);
            time = static_cast<double>(std::chrono::duration_cast<
                    std::chrono::nanoseconds>(std::chrono::system_clock::now() - s1).count()) / 1e9;
        }
        else if (benchmark == BenchmarkTypes::kRangeSearch)
        {
            s1 = std::chrono::system_clock::now();
            structure->RangeSearch(search_numbers);
            time = static_cast<double>(std::chrono::duration_cast<
                    std::chrono::nanoseconds>(std::chrono::system_clock::now() - s1).count()) / 1e9;
        }

        if (verbose)
            std::cout << "Finished " << name << ". Deleting..." << std::endl;

        structure_times[i] = time;
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
            number_elements = 256'000'000;
    }

    auto t1 = std::chrono::system_clock::now();

    std::cout << "Starting '" << benchmark_to_string() << "' benchmark with size '" << size << "' (" << number_elements
              << " keys), '" << iterations << "' iterations and '" << (dense ? "dense" : "sparse") << "' keys.\n" <<
              std::endl;

    std::vector<std::tuple<double, double, double, double>> structure_times(kIndexStructures.size());

    // Run Benchmarks
    for (uint32_t i = 0; i < iterations; ++i)
    {
        if (verbose)
            std::cout << "Running iteration " << (i + 1) << "/" << iterations << "..." << std::endl;
        AggregateBenchmarkResults(structure_times, RunBenchmarkIteration(), i);
    }

    const auto time = static_cast<double>(std::chrono::duration_cast<
            std::chrono::seconds>(std::chrono::system_clock::now() - t1).count()) / 60;
    std::cout << "Finished '" << benchmark_to_string() << "' benchmark with size '" << size << "' (" << number_elements
              << " keys), '" <<
              iterations << "' iterations and '" << (dense ? "dense" : "sparse") << "' keys in " << std::fixed
              << std::setprecision(1) << time << " minutes.\n"
              << std::endl;

    std::cout << "=================================================================================" << std::endl;
    std::cout << "\t\t\t\tBENCHMARK RESULTS" << std::endl;
    std::cout << "=================================================================================" << std::endl;

    std::cout << "Index Structure\t|      Min\t|      Max\t|      Avg\t|      M Ops/s\t|" << std::endl;
    std::cout << "---------------------------------------------------------------------------------" << std::endl;

    std::cout.precision(4);

    for (uint32_t i = 0; i < kIndexStructures.size(); ++i)
    {
        const auto& [name, spacing, structure] = kIndexStructures[i];

        if (skip.contains(name)) continue;

        auto& times = structure_times[i];

        std::cout << name;
        for (uint8_t j = 0; j < spacing; ++j)
        {
            std::cout << "\t";
        }
        std::cout << "|" << GetDoubleOffset(std::get<0>(times));

        std::cout << std::fixed
                  << std::get<0>(times) << "s\t|" << GetDoubleOffset(std::get<1>(times))
                  << std::get<1>(times) << "s\t|" << GetDoubleOffset(std::get<2>(times))
                  << std::get<2>(times) << "s\t|" << GetDoubleOffset(number_elements / std::get<2>(times) / 1e6)
                  << number_elements / std::get<2>(times) / 1e6 << "\t|\t"
                  << std::endl;

        // Delete Structure Benchmark
        delete structure;
    }
}

int main(int argc, char* argv[])
{
    if (CmdArgExists(argv, argv + argc, "-h"))
    {
        printf(kHelpMsg, argv[0], kDefaultIterations);
        return EXIT_SUCCESS;
    }

    if constexpr (sizeof(intptr_t) != 8)
    {
        std::cerr << "Please make sure to compile this program for 64 bit architecture!" << std::endl;
        return EXIT_FAILURE;
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
    char* skip_arg = GetCmdArg(argv, argv + argc, "--skip");

    // TODO: Default Values

    if (benchmark_arg == nullptr || size_arg == nullptr)
    {
        fprintf(stderr, kUsageMsg, argv[0]);
        return EXIT_FAILURE;
    }

    const std::string benchmark_str{benchmark_arg};
    const std::string size_str{size_arg};

    //const std::string benchmark_str{"range_search"};
    //const std::string size_str{"3"};


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

        // skip structures not supporting range queries
        skip.insert("H-Trie");
        skip.insert("Hash-Table");
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

    if (skip_arg != nullptr)
    {
        const std::string skip_str{skip_arg};

        size_t last = 0;
        size_t next;
        while ((next = skip_str.find(',', last)) != std::string::npos)
        {
            skip.insert(skip_str.substr(last, next - last));
            last = next + 1;
        }
        skip.insert(skip_str.substr(last));
    }

    dense = CmdArgExists(argv, argv + argc, "-d");
    // TODO:
    //dense = false;

    verbose = CmdArgExists(argv, argv + argc, "-v");

    /**
     * Run Benchmark.
     */
    RunBenchmark();

    return EXIT_SUCCESS;
}
