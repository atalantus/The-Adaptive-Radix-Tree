# ART Implementation (C++)

## Usage
To run the project compile it on a x86-64 architecture supporting at least SSE2 with either MSVC++ or GCC-11 (or later).

# Project Structure

## Benchmark
Includes various performance benchmarks for the data structures.

## Test
Includes testing to verify the data structures are implemented correctly.

## Bin
Includes different Shell scripts to automatically run different sets of benchmarks.

## Data Structures
Various data structures for storing unique 32 bit keys.

Note: Since keys are compactly represented as uint32_t types and traversed using bit shifts we actually don't require
any key transformations storing keys in Tries even for little-endian architectures.

### ART
**ART Implementation without path compression.**

**Notes:**
- Without path compression all nodes only store a 2 byte header (1 Byte node type, 1 Byte number of non-null children)
- SIMD comparison for Node16 (SSE2 x86-64 specific)
- Combined value/pointer slots using pointer tagging (64 bit architecture specific)

### Trie
**A 256-way trie storing children in a 256 sized child pointer array.**

The implementation is specific for 32 bit keys and uses pointer tagging to store the last byte as a child pointer.
### M-Trie
**A 256-way trie storing children in an ordered map.**

The implementation is specific for 32 bit keys and uses pointer tagging to store the last byte as a child pointer.

### H-Trie
**A 256-way trie stored in a single hash-table.**

Does not support range queries.

### Sorted List
**Stores a specific range of keys in a sorted list.**

### Hash-Table
**Standard Library Hash-Table.**

The implementation uses `std::unordered_set`.

Does not support range queries.

### RB-Tree
**Standard Library Red-black Tree.**

The implementation uses `std::set`.

# Benchmark Results
The following results were benchmarked on an Intel Core i5-8400 CPU with around 3.70GHz speed during benchmarking with 16GB DDR4-2666MHz RAM.

The code was compiled with MSVC v143. (There might be some not too insignificant performance differences compared to a GCC -O3 compilation.)

Note that for benchmarks of size 2 with sparse keys the Trie structure is not included due to exessive memory consumption
as well as H-Trie and Hash-Table for any range_search benchmark since these two don't really support range queries.

### Insert
```
Finished 'insert' benchmark with size '1' (65000 keys), '10' iterations and 'dense' keys in 0 minutes.

=================================================================================
                                BENCHMARK RESULTS
=================================================================================
Index Structure |      Min      |      Max      |      Avg      |      M Ops/s  |
---------------------------------------------------------------------------------
ART             |      0.0020s  |      0.0021s  |      0.0020s  |     31.8079   |
Trie            |      0.0005s  |      0.0007s  |      0.0006s  |    111.8087   |
M-Trie          |      0.0172s  |      0.0178s  |      0.0174s  |      3.7317   |
H-Trie          |      0.0144s  |      0.0158s  |      0.0151s  |      4.3044   |
Sorted List     |      0.0001s  |      0.0001s  |      0.0001s  |    563.9424   |
Hash-Table      |      0.0042s  |      0.0046s  |      0.0044s  |     14.8138   |
RB-Tree         |      0.0122s  |      0.0124s  |      0.0123s  |      5.2766   |



Finished 'insert' benchmark with size '1' (65000 keys), '10' iterations and 'sparse' keys in 0.05 minutes.

=================================================================================
                                BENCHMARK RESULTS
=================================================================================
Index Structure |      Min      |      Max      |      Avg      |      M Ops/s  |
---------------------------------------------------------------------------------
ART             |      0.0030s  |      0.0033s  |      0.0031s  |     20.9964   |
Trie            |      0.1014s  |      0.1040s  |      0.1028s  |      0.6322   |
M-Trie          |      0.0487s  |      0.0535s  |      0.0500s  |      1.2994   |
H-Trie          |      0.0569s  |      0.0623s  |      0.0592s  |      1.0974   |
Sorted List     |      0.0001s  |      0.0001s  |      0.0001s  |    574.4079   |
Hash-Table      |      0.0057s  |      0.0075s  |      0.0060s  |     10.8753   |
RB-Tree         |      0.0148s  |      0.0160s  |      0.0152s  |      4.2733   |
```

### Search
```
Finished 'search' benchmark with size '1' (65000 keys), '10' iterations and 'dense' keys in 0.02 minutes.

=================================================================================
                                BENCHMARK RESULTS
=================================================================================
Index Structure |      Min      |      Max      |      Avg      |      M Ops/s  |
---------------------------------------------------------------------------------
ART             |      0.0011s  |      0.0012s  |      0.0011s  |     58.5401   |
Trie            |      0.0003s  |      0.0004s  |      0.0003s  |    195.6535   |
M-Trie          |      0.0142s  |      0.0147s  |      0.0144s  |      4.5051   |
H-Trie          |      0.0093s  |      0.0109s  |      0.0101s  |      6.4541   |
Sorted List     |      0.0017s  |      0.0020s  |      0.0018s  |     35.9027   |
Hash-Table      |      0.0006s  |      0.0006s  |      0.0006s  |    113.4281   |
RB-Tree         |      0.0099s  |      0.0100s  |      0.0100s  |      6.5245   |



Finished 'search' benchmark with size '1' (65000 keys), '10' iterations and 'sparse' keys in 0.07 minutes.

=================================================================================
                                BENCHMARK RESULTS
=================================================================================
Index Structure |      Min      |      Max      |      Avg      |      M Ops/s  |
---------------------------------------------------------------------------------
ART             |      0.0016s  |      0.0022s  |      0.0017s  |     39.0449   |
Trie            |      0.0036s  |      0.0039s  |      0.0037s  |     17.3418   |
M-Trie          |      0.0413s  |      0.0427s  |      0.0419s  |      1.5504   |
H-Trie          |      0.0340s  |      0.0365s  |      0.0350s  |      1.8584   |
Sorted List     |      0.0016s  |      0.0021s  |      0.0018s  |     35.7434   |
Hash-Table      |      0.0011s  |      0.0011s  |      0.0011s  |     59.3093   |
RB-Tree         |      0.0111s  |      0.0115s  |      0.0113s  |      5.7611   |
```

### Range Search
```
```
