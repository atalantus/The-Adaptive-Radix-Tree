#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>
#include <string>
#include <chrono>
#include "benchmark.h"
#include "data_structures.h"
#include "benchmark_util.h"

constexpr char kUsageMsg[] =
        "usage: %s [-h] -b benchmark -s size [-i number_iterations] [-d] [--only structure_list] [--skip structure_list] [--seed seed_number] [-v]\n";
constexpr char kHelpMsg[] = "This program benchmarks different indexing structures using 32 bit unsigned integers. "
        "For the specified benchmark and size the benchmark is run number_iterations times for each "
        "index structure and the min, max and average times are outputted.\n\n"
        "usage: %s [-h] -b benchmark -s size [-i number_iterations] [-d] [--only structure_list] [--skip structure_list] [--seed seed_number] [-v]\n"
        "\nThe parameters in detail:\n"
        "\t-h\t\t\t\t: Shows how to use the program (this text).\n"
        "\t-b <insert/search/range_search>\t: Specifies the benchmark to run. You can either benchmark insertion, searching or searching in range.\n"
        "\t-s <1/2/3>\t\t\t: Specifies the benchmark size. Options are 1 with 65 thousand integers, 2 with 16 million integers and 3 with 256 million integers.\n"
        "\t-i <number>\t\t\t: Specifies the number of iterations the benchmark is run. Default value is %u. Should be an integer between 1 and 10000 (inclusive).\n"
        "\t-d\t\t\t\t: Use a dense (from 0 up to number of elements - 1) set of integers as keys. Otherwise a sparse (uniform random 32 bit integer) set will be used.\n"
        "\t--only <structure_list>\t\t\t: Specifies index structures to be used during this benchmark. Given as comma separated list of names (ART, ART (Exp), ART (Leis), Trie, M-Trie, H-Trie, Sorted List, Hash-Table, RB-Tree). If not set all index structures will be used.\n"
        "\t--skip <structure_list>\t\t\t: Specifies index structures to be skipped during this benchmark. Given as comma separated list of names (ART, ART (Exp), ART (Leis), Trie, M-Trie, H-Trie, Sorted List, Hash-Table, RB-Tree).\n"
        "\t--seed <seed_number>\t\t\t: Use deterministic values by starting first benchmark iteration with a given seed and all subsequent iterations with increasing seeds. If not set all iterations will use a random seed.\n"
        "\t-v\t\t\t\t: Enable verbose logging.\n";

/**
 * List of Index Structures each with a name, the number of tabs after the name (used for printing benchmark table)
 * and it's own Benchmark object.
 */
const std::vector<std::tuple<std::string, uint8_t, Benchmark*>> kIndexStructures{
        {"ART", 2, new ArtBenchmark()},
        {"ART (Exp)", 1, new ArtExpBenchmark()},
        {"ART (Leis)", 1, new ArtLeisBenchmark()},
        {"Trie", 2, new TrieBenchmark()},
        {"M-Trie", 2, new MTrieBenchmark()},
        //{"H-Trie", 2, new HTrieBenchmark()},
        {"Sorted List", 1, new SortedListBenchmark()},
        {"Hash-Table", 1, new HashTableBenchmark()},
        {"RB-Tree", 2, new RbTreeBenchmark()}
};

constexpr uint32_t kDefaultIterations{3};

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
bool custom_seed = false;
bool verbose = false;

size_t seed = -1;

#ifdef TRACK_MEMORY

struct MemoryAllocator
{
    uint64_t total_allocated = 0;
    uint64_t total_freed = 0;

    uint64_t GetMemoryUsage() const
    {
        return total_allocated - total_freed;
    }

    void Reset()
    {
        if (total_allocated - total_freed != 0)
        {
            std::cerr << "\033[1;31mPossible Memory Leak detected! total_allocated: " << total_allocated << ", total_freed: " << total_freed
                    << ", Difference: " << total_allocated - total_freed << "\033[0m" << std::endl;
        }

        total_allocated = 0;
        total_freed = 0;
    }
};

static MemoryAllocator memory_allocator;

void* operator new(size_t size)
{
    memory_allocator.total_allocated += size;

    return malloc(size);
}

void operator delete(void* memory, size_t size)
{
    memory_allocator.total_freed += size;

    free(memory);
}

#endif

void GenerateRandomNumbers(std::vector<uint32_t>& numbers, std::vector<uint32_t>& search_numbers)
{
    std::random_device rnd;
    seed = custom_seed ? ++seed : rnd();
    std::mt19937_64 eng(seed);
    const uint32_t s = dense ? number_elements - 1 : 4'294'967'295;
    std::uniform_int_distribution<uint32_t> numbers_distr(0, s);
    std::uniform_int_distribution<uint32_t> search_numbers_distr(0, number_elements - 1);

    numbers.reserve(number_elements);

    for (uint32_t i = 0; i < number_elements; ++i)
    {
        numbers.push_back(numbers_distr(eng));
    }

    if (benchmark == BenchmarkTypes::kSearch)
    {
        search_numbers.reserve(number_elements);

        for (uint32_t i = 0; i < number_elements; ++i)
            search_numbers.push_back(numbers[search_numbers_distr(eng)]);
    }
    else if (benchmark == BenchmarkTypes::kRangeSearch)
    {
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

auto RunBenchmarkIteration()
{
    std::vector<double> structure_times(kIndexStructures.size());

    std::vector<uint32_t> numbers;
    std::vector<uint32_t> search_numbers;

    GenerateRandomNumbers(numbers, search_numbers);

    if (verbose)
        std::cout << "Finished allocating Memory for Numbers." << std::endl;

#ifdef TRACK_MEMORY
    memory_allocator.total_allocated = 0;
    memory_allocator.total_freed = 0;
#endif

    /**
     * Benchmark Index Structures
     */
    for (uint32_t i = 0; i < kIndexStructures.size(); ++i)
    {
        const auto& [name, _, structure] = kIndexStructures[i];
        // result used for this benchmark (runtime in seconds or memory used in bytes)
        double result = 0.0;

        if (skip.contains(name)) continue;

        const auto t0 = std::chrono::system_clock::now();

#ifdef TRACK_MEMORY
        memory_allocator.Reset();
#endif

        structure->InitializeStructure();

        auto t1 = std::chrono::system_clock::now();
        structure->Insert(numbers);
#ifdef TRACK_MEMORY
        result = static_cast<double>(memory_allocator.GetMemoryUsage());
#else
        result = static_cast<double>(std::chrono::duration_cast<
            std::chrono::nanoseconds>(std::chrono::system_clock::now() - t1).count()) / 1e9;
#endif

        if (benchmark == BenchmarkTypes::kSearch)
        {
            t1 = std::chrono::system_clock::now();
            structure->Search(search_numbers);
#ifdef TRACK_MEMORY
            result = static_cast<double>(memory_allocator.GetMemoryUsage());
#else
            result = static_cast<double>(std::chrono::duration_cast<
                std::chrono::nanoseconds>(std::chrono::system_clock::now() - t1).count()) / 1e9;
#endif
        }
        else if (benchmark == BenchmarkTypes::kRangeSearch)
        {
            t1 = std::chrono::system_clock::now();
            structure->RangeSearch(search_numbers);
#ifdef TRACK_MEMORY
            result = static_cast<double>(memory_allocator.GetMemoryUsage());
#else
            result = static_cast<double>(std::chrono::duration_cast<
                std::chrono::nanoseconds>(std::chrono::system_clock::now() - t1).count()) / 1e9;
#endif
        }

        if (verbose)
            std::cout << "Finished " << name << " in " << std::fixed << std::setprecision(1)
            << static_cast<double>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - t0).count()) / 60 
            << " minutes." << std::endl;

        structure_times[i] = result;
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

    const auto t1 = std::chrono::system_clock::now();

    std::cout << "Starting '" << benchmark_to_string() << "' benchmark with size '" << size << "' (" << number_elements
            << " keys), '" << iterations << "' iterations and '" << (dense ? "dense" : "sparse") << "' keys." <<
            std::endl;

    std::vector structure_times(kIndexStructures.size(), std::vector<double>(iterations));

    // Run Benchmarks
    for (uint32_t i = 0; i < iterations; ++i)
    {
        if (verbose)
            std::cout << "\nRunning iteration " << (i + 1) << "/" << iterations << " with seed " << seed << "..." << std::endl;
        else
            std::cout << '\r' << "Running iteration " << (i + 1) << "/" << iterations << " with seed " << seed << "..." << std::flush;

        auto times = RunBenchmarkIteration();

        for (uint32_t j = 0; j < kIndexStructures.size(); ++j)
            structure_times[j][i] = times[j];
    }

    const auto time = static_cast<double>(std::chrono::duration_cast<
        std::chrono::seconds>(std::chrono::system_clock::now() - t1).count()) / 60;
    std::cout << "\nFinished '" << benchmark_to_string() << "' benchmark with size '" << size << "' (" << number_elements
            << " keys), '" <<
            iterations << "' iterations and '" << (dense ? "dense" : "sparse") << "' keys in " << std::fixed
            << std::setprecision(1) << time << " minutes.\n"
            << std::endl;

#ifdef TRACK_MEMORY
    std::cout << "=================================================================================================================" <<
            std::endl;
    std::cout << "\t\t\t\t\tMEMORY BENCHMARK RESULTS" << std::endl;
    std::cout << "=================================================================================================================" <<
            std::endl;

    std::cout << "Index Structure\t|         Min\t\t|         Max\t\t|         Avg\t\t|         Med\t\t|" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------------------------------------" <<
            std::endl;
#else
    std::cout << "=================================================================================================================" <<
            std::endl;
    std::cout << "\t\t\t\t\tPERFORMANCE BENCHMARK RESULTS" << std::endl;
    std::cout << "=================================================================================================================" <<
            std::endl;

    std::cout << "Index Structure\t|      Min\t|      Max\t|      Avg\t|      Med\t| M Ops/s (Avg)\t| M Ops/s (Med)\t|" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------------------------------------" <<
            std::endl;
#endif

    for (uint32_t i = 0; i < kIndexStructures.size(); ++i)
    {
        const auto& [name, spacing, structure] = kIndexStructures[i];

        if (skip.contains(name)) continue;

        auto& times = structure_times[i];
        std::ranges::sort(times);

        const double min = times[0];
        const double max = times[times.size() - 1];
        double sum = 0.0;
        for (const auto& t : times)
            sum += t;
        const double avg = sum / times.size();
        const double med = times.size() % 2 == 1
                               ? times[times.size() / 2]
                               : (times[times.size() / 2] + times[times.size() / 2 + 1]) / 2.0;

        std::cout << name;
        for (uint8_t j = 0; j < spacing; ++j)
        {
            std::cout << "\t";
        }
#ifdef TRACK_MEMORY
        std::cout << "|"
                << FormatMemory(static_cast<uint64_t>(min)) << FormatMemory(static_cast<uint64_t>(max))
                << FormatMemory(static_cast<uint64_t>(avg)) << FormatMemory(static_cast<uint64_t>(med))
                << std::endl;
#else
        const double avg_ops = number_elements / avg / 1e6;
        const double med_ops = number_elements / med / 1e6;

        std::cout << "|"
                << FormatTime(min, true) << FormatTime(max, true)
                << FormatTime(avg, true) << FormatTime(med, true)
                << FormatTime(avg_ops, false) << FormatTime(med_ops, false)
                << std::endl;
#endif

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

    char* benchmark_arg = GetCmdArg(argv, argv + argc, "-b");
    char* size_arg = GetCmdArg(argv, argv + argc, "-s");
    char* iterations_arg = GetCmdArg(argv, argv + argc, "-i");
    char* only_arg = GetCmdArg(argv, argv + argc, "--only");
    char* skip_arg = GetCmdArg(argv, argv + argc, "--skip");
    char* seed_arg = GetCmdArg(argv, argv + argc, "--seed");

    if (benchmark_arg == nullptr || size_arg == nullptr)
    {
        fprintf(stderr, kUsageMsg, argv[0]);
        return EXIT_FAILURE;
    }

    const std::string benchmark_str{benchmark_arg};
    const std::string size_str{size_arg};

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
        skip.insert("ART (Leis)");
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

    if (only_arg != nullptr)
    {
        const std::string only_str{only_arg};
        std::set<std::string> only;

        size_t last = 0;
        size_t next;
        while ((next = only_str.find(',', last)) != std::string::npos)
        {
            only.insert(only_str.substr(last, next - last));
            last = next + 1;
        }
        only.insert(only_str.substr(last));

        // skip all structures not in only set
        for (const auto& s : kIndexStructures)
        {
            if (only.contains(get<0>(s))) continue;
            skip.insert(get<0>(s));
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

    if (seed_arg != nullptr)
    {
        custom_seed = true;
        const std::string seed_str{seed_arg};

        try
        {
            seed = std::stoul(seed_str);
        }
        catch (std::logic_error&)
        {
            std::cerr << "Invalid 'seed' argument \"" << seed_str
                    << "\". Expected positive integer." << std::endl;
            return EXIT_FAILURE;
        }

        if (seed < 1)
        {
            std::cerr << "Invalid 'seed' argument \"" << seed_str
                    << "\". Expected positive integer." << std::endl;
            return EXIT_FAILURE;
        }

        seed--;
    }

    dense = CmdArgExists(argv, argv + argc, "-d");
    verbose = CmdArgExists(argv, argv + argc, "-v");

    /**
     * Run Benchmark.
     */
    RunBenchmark();

    return EXIT_SUCCESS;
}
