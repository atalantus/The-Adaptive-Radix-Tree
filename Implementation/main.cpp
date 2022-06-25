#include <algorithm>
#include <iostream>
#include <random>
#include <functional>
#include <string>
#include <chrono>
#include "data_structures/data_structures.h"
#include "benchmark.h"

constexpr char usage_msg[] = "usage: %s [-h] -b benchmark -s size [-i number_iterations]\n";
constexpr char help_msg[] = "This program benchmarks different indexing structures using 32 bit unsigned integers. "
	"For the specified benchmark and size the benchmark is run number_iterations times for each "
	"index structure and the min, max and average times are outputted.\n\n"
	"usage: %s [-h] -b benchmark -s size [-i number_iterations]\n"
	"\nThe parameters in detail:\n"
	"\t-h\t\t\t\t: Shows how to use the program (this text).\n"
	"\t-b <insert/search/range_search>\t: Specifies the benchmark to run. You can either benchmark insertion, searching or searching in range.\n"
	"\t-s <1/2/3>\t\t\t: Specifies the benchmark size. Options are 1 with 65 thousand integers, 2 with 16 million integers and 3 with 256 million integers.\n"
	"\t-i <number>\t\t\t: Specifies the number of iterations the benchmark is run. Default value is %u. Should be an integer between 1 and 10000 (inclusive).\n";

const std::vector<std::string> index_structures{"ART", "Trie"};

constexpr uint32_t default_iterations{10};


enum class benchmark
{
	insert,
	search,
	range_search
};

void aggregate_benchmark_results(auto& total, const std::vector<double>& results, const uint32_t iteration)
{
	for (uint32_t i = 0; i < index_structures.size(); ++i)
	{
		// first iteration
		if (iteration == 0)
		{
			total[i] = {results[i], results[i], results[i], results[i]};
			continue;
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

double test_index_structure(const std::function<void(std::vector<uint32_t>)>& benchmark,
                            const std::vector<uint32_t>& numbers)
{
	const auto t1 = std::chrono::high_resolution_clock::now();

	benchmark(numbers);

	const auto t2 = std::chrono::high_resolution_clock::now();

	const std::chrono::duration<double, std::milli> dur{t2 - t1};

	return dur.count() / 1000.0;
}

auto run_benchmark_iteration(const benchmark benchmark, const uint32_t size)
{
	std::vector<double> structure_times(index_structures.size());

	/*
	 * Pre-Generate Random Numbers
	 */
	std::random_device rnd;
	std::mt19937_64 eng(rnd());
	const std::uniform_int_distribution<uint32_t> distr;

	std::vector<uint32_t> numbers(size);

	for (auto& n : numbers)
	{
		n = distr(eng);
	}

	switch (benchmark)
	{
	case benchmark::insert:
		structure_times[0] = test_index_structure({art_insert}, numbers);
		structure_times[1] = test_index_structure({trie_insert}, numbers);
		break;
	case benchmark::search:
		break;
	case benchmark::range_search:
		break;
	}

	return structure_times;
}

void run_benchmark(benchmark benchmark, uint32_t size, const uint32_t iterations)
{
	auto benchmark_to_string = [benchmark]()
	{
		switch (benchmark)
		{
		case benchmark::insert:
			return "insert";
		case benchmark::search:
			return "search";
		case benchmark::range_search:
			return "range_search";
		}
	};

	std::cout << "Starting '" << benchmark_to_string() << "' benchmark with size '" << size << "' and '" << iterations
		<< "' iterations.\n" << std::endl;

	switch (size)
	{
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

	std::vector<std::tuple<double, double, double, double>> structureTimes(index_structures.size());

	// Run Benchmarks
	for (uint32_t i = 0; i < iterations; ++i)
	{
		std::cout << "Running iteration " << (i + 1) << "/" << iterations << "..." << std::endl;
		aggregate_benchmark_results(structureTimes, run_benchmark_iteration(benchmark, size), i);
	}

	std::cout << "\nFinished running '" << benchmark_to_string() << "' benchmark.\n" << std::endl;

	std::cout << "=================================================================" << std::endl;
	std::cout << "\t\t\tBENCHMARK RESULTS\t\t\t" << std::endl;
	std::cout << "=================================================================" << std::endl;

	std::cout << "Index Structure\t|\tMin\t|\tMax\t|\tAvg\t|" << std::endl;
	std::cout << "-----------------------------------------------------------------" << std::endl;

	std::cout.precision(2);

	for (uint32_t i = 0; i < index_structures.size(); ++i)
	{
		auto& times = structureTimes[i];

		std::cout << std::fixed << index_structures[i] << "\t\t|\t"
			<< std::get<0>(times) << "s\t|\t"
			<< std::get<1>(times) << "s\t|\t"
			<< std::get<2>(times) << "s\t|\t" << std::endl;
	}
}

char* get_cmd_arg(char** begin, char** end, const std::string& arg)
{
	char** itr = std::find(begin, end, arg);
	if (itr != end && ++itr != end)
		return *itr;
	return nullptr;
}

bool cmd_arg_exists(char** begin, char** end, const std::string& arg)
{
	return std::find(begin, end, arg) != end;
}

int main(int argc, char* argv[])
{
	if (cmd_arg_exists(argv, argv + argc, "-h"))
	{
		printf(help_msg, argv[0], default_iterations);
		return EXIT_SUCCESS;
	}

	/*
	if (argc < 5)
	{
		fprintf(stderr, usage_msg, argv[0]);
		return EXIT_FAILURE;
	}
	*/

	char* benchmark_arg = get_cmd_arg(argv, argv + argc, "-b");
	char* size_arg = get_cmd_arg(argv, argv + argc, "-s");
	char* iterations_arg = get_cmd_arg(argv, argv + argc, "-i");

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
	benchmark benchmark;
	uint32_t size = 0;
	uint32_t iterations{default_iterations};

	if (benchmark_str == "insert")
	{
		benchmark = benchmark::insert;
	}
	else if (benchmark_str == "search")
	{
		benchmark = benchmark::search;
	}
	else if (benchmark_str == "range_search")
	{
		benchmark = benchmark::range_search;
	}
	else
	{
		std::cerr << "Unknown 'benchmark' argument \"" << benchmark_str
			<< R"(". Possible options are "insert", "search" and "range_search".)" << std::endl;
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
	run_benchmark(benchmark, size, iterations);

	return EXIT_SUCCESS;
}
