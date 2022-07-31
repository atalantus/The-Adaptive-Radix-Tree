# ART Implementation (C++)

### Usage
To run the project compile it on a x86-64 architecture supporting at least SSE2 with either MSVC++ or GCC-11 (or later).

## Project Structure

### Benchmark
Includes various performance benchmarks for the data structures.

### Test
Includes testing to verify the data structures are implemented correctly.

### Bin
Includes different Shell scripts to automatically run different sets of benchmarks.

### Data Structures
Various data structures for storing unique 32 bit keys.

Note: Since keys are compactly represented as uint32_t types and traversed using bit shifts we actually don't require
any key transformations storing keys in Tries even for little-endian architectures.

#### ART
**ART Implementation without path compression.**

**Notes:**
- Deriving from an abstract base class and calling Node functions via dynamic dispatch was faster than manual switch case with static_cast (See branch `dynamic_dispatch_vs_static_cast` TODO)
- Without path compression all nodes only store a 2 byte header (1 Byte node type, 1 Byte number of non-null children)
- SIMD comparison for Node16 (SSE2 x86-64 specific)
- Combined value/pointer slots using pointer tagging (64 bit architecture specific)

#### Trie
**A 256-way trie storing children in a 256 sized child pointer array.**

The implementation is specific for 32 bit keys and uses pointer tagging to store the last byte as a child pointer.
### M-Trie
**A 256-way trie storing children in an ordered map.**

The implementation is specific for 32 bit keys and uses pointer tagging to store the last byte as a child pointer.

#### H-Trie
**A 256-way trie stored in a single hash-table.**

Does not support range queries.

#### Sorted List
**Stores a specific range of keys in a sorted list.**

#### Hash-Table
**Standard Library Hash-Table.**

The implementation uses `std::unordered_set`.

Does not support range queries.

#### RB-Tree
**Standard Library Red-black Tree.**

The implementation uses `std::set`.

## Benchmark Results
The following results were benchmarked on an Intel Core i5-8400 CPU with around 3.70GHz speed during benchmarking with 16GB DDR4-2666MHz RAM.

The code was compiled with MSVC v143. (There might be some performance differences compared to a GCC compilation.)

Note that for benchmarks of size 2 with sparse keys the Trie structure is not included due to exessive memory consumption
as well as H-Trie and Hash-Table for any range_search benchmark since these two don't really support range queries.

Dense keys only range from 0 to total number of keys - 1 while sparse keys are uniformly distributed 32 bit integers.

### Insert
Insertion of uniformly (in the population) distributed keys.

H-Trie results are currently omitted due to implementation errors.
```
Finished 'insert' benchmark with size '1' (65000 keys), '10' iterations and 'dense' keys in 0.0 minutes.

=================================================================================
                                BENCHMARK RESULTS
=================================================================================
Index Structure |      Min      |      Max      |      Avg      |      M Ops/s  |
---------------------------------------------------------------------------------
ART             |      0.0020s  |      0.0024s  |      0.0022s  |     29.7468   |
Trie            |      0.0006s  |      0.0007s  |      0.0007s  |     98.4789   |
M-Trie          |      0.0172s  |      0.0206s  |      0.0184s  |      3.5240   |
Sorted List     |      0.0040s  |      0.0046s  |      0.0042s  |     15.5222   |
Hash-Table      |      0.0045s  |      0.0054s  |      0.0048s  |     13.6128   |
RB-Tree         |      0.0124s  |      0.0164s  |      0.0136s  |      4.7717   |



Finished 'insert' benchmark with size '1' (65000 keys), '10' iterations and 'sparse' keys in 0.1 minutes.

=================================================================================
                                BENCHMARK RESULTS
=================================================================================
Index Structure |      Min      |      Max      |      Avg      |      M Ops/s  |
---------------------------------------------------------------------------------
ART             |      0.0031s  |      0.0035s  |      0.0032s  |     20.1500   |
Trie            |      0.1032s  |      0.1111s  |      0.1068s  |      0.6087   |
M-Trie          |      0.0503s  |      0.0570s  |      0.0529s  |      1.2280   |
Sorted List     |      0.0039s  |      0.0041s  |      0.0039s  |     16.4702   |
Hash-Table      |      0.0059s  |      0.0066s  |      0.0062s  |     10.5505   |
RB-Tree         |      0.0154s  |      0.0166s  |      0.0158s  |      4.1051   |



Finished 'insert' benchmark with size '2' (16000000 keys), '5' iterations and 'dense' keys in 4.3 minutes.

=================================================================================
                                BENCHMARK RESULTS
=================================================================================
Index Structure |      Min      |      Max      |      Avg      |      M Ops/s  |
---------------------------------------------------------------------------------
ART             |      1.3634s  |      1.4123s  |      1.3779s  |     11.6117   |
Trie            |      0.5873s  |      0.6286s  |      0.6055s  |     26.4237   |
M-Trie          |     22.4466s  |     23.4110s  |     22.6582s  |      0.7061   |
Sorted List     |      1.4786s  |      1.4889s  |      1.4829s  |     10.7894   |
Hash-Table      |      3.7238s  |      3.7728s  |      3.7504s  |      4.2662   |
RB-Tree         |     16.0597s  |     16.1068s  |     16.0799s  |      0.9950   |



Finished 'insert' benchmark with size '2' (16000000 keys), '5' iterations and 'sparse' keys in 6.5 minutes.

=================================================================================
                                BENCHMARK RESULTS
=================================================================================
Index Structure |      Min      |      Max      |      Avg      |      M Ops/s  |
---------------------------------------------------------------------------------
ART             |      1.9125s  |      2.0691s  |      1.9862s  |      8.0555   |
M-Trie          |     30.7611s  |     31.4636s  |     31.1535s  |      0.5136   |
Sorted List     |      1.4358s  |      1.4448s  |      1.4403s  |     11.1086   |
Hash-Table      |      5.0158s  |      5.2396s  |      5.1024s  |      3.1358   |
RB-Tree         |     18.3403s  |     18.8331s  |     18.6312s  |      0.8588   |
```

### Search
Positive single-point lookups of uniformly (in the population) distributed keys.

H-Trie results are currently omitted due to implementation errors.
```
Finished 'search' benchmark with size '1' (65000 keys), '10' iterations and 'dense' keys in 0.0 minutes.

=================================================================================
                                BENCHMARK RESULTS
=================================================================================
Index Structure |      Min      |      Max      |      Avg      |      M Ops/s  |
---------------------------------------------------------------------------------
ART             |      0.0012s  |      0.0014s  |      0.0012s  |     52.8705   |
Trie            |      0.0003s  |      0.0004s  |      0.0003s  |    185.9002   |
M-Trie          |      0.0175s  |      0.0182s  |      0.0177s  |      3.6698   |
Sorted List     |      0.0049s  |      0.0050s  |      0.0049s  |     13.1701   |
Hash-Table      |      0.0006s  |      0.0007s  |      0.0006s  |    102.9246   |
RB-Tree         |      0.0102s  |      0.0105s  |      0.0103s  |      6.3135   |



Finished 'search' benchmark with size '1' (65000 keys), '10' iterations and 'sparse' keys in 0.1 minutes.

=================================================================================
                                BENCHMARK RESULTS
=================================================================================
Index Structure |      Min      |      Max      |      Avg      |      M Ops/s  |
---------------------------------------------------------------------------------
ART             |      0.0018s  |      0.0020s  |      0.0018s  |     35.3811   |
Trie            |      0.0037s  |      0.0039s  |      0.0038s  |     16.9914   |
M-Trie          |      0.0430s  |      0.0463s  |      0.0439s  |      1.4793   |
Sorted List     |      0.0052s  |      0.0054s  |      0.0053s  |     12.2877   |
Hash-Table      |      0.0011s  |      0.0012s  |      0.0012s  |     55.8837   |
RB-Tree         |      0.0114s  |      0.0117s  |      0.0116s  |      5.6182   |



Finished 'search' benchmark with size '2' (16000000 keys), '5' iterations and 'dense' keys in 8.3 minutes.

=================================================================================
                                BENCHMARK RESULTS
=================================================================================
Index Structure |      Min      |      Max      |      Avg      |      M Ops/s  |
---------------------------------------------------------------------------------
ART             |      1.1186s  |      1.1522s  |      1.1315s  |     14.1399   |
Trie            |      0.4607s  |      0.4891s  |      0.4749s  |     33.6899   |
M-Trie          |     23.7496s  |     24.0047s  |     23.8522s  |      0.6708   |
Sorted List     |      4.4650s  |      4.5356s  |      4.4866s  |      3.5662   |
Hash-Table      |      0.8449s  |      0.8609s  |      0.8539s  |     18.7377   |
RB-Tree         |     16.7894s  |     16.9299s  |     16.8539s  |      0.9493   |



Finished 'search' benchmark with size '2' (16000000 keys), '5' iterations and 'sparse' keys in 11.5 minutes.

=================================================================================
                                BENCHMARK RESULTS
=================================================================================
Index Structure |      Min      |      Max      |      Avg      |      M Ops/s  |
---------------------------------------------------------------------------------
ART             |      1.7436s  |      1.7966s  |      1.7806s  |      8.9859   |
M-Trie          |     31.4560s  |     32.1922s  |     31.6870s  |      0.5049   |
Sorted List     |      4.9158s  |      4.9655s  |      4.9406s  |      3.2385   |
Hash-Table      |      1.0988s  |      1.1354s  |      1.1153s  |     14.3458   |
RB-Tree         |     19.0503s  |     19.1860s  |     19.1106s  |      0.8372   |
```

### Range Search
Index structures not supporting range queries were omitted (H-Trie, Hash-Table).

Executes 25600 range queries each with its lower and upper bound key uniformly (in the population) distributed.

Note that currently the results for ART, Trie and M-Trie may be worse than actuality because of inperformant
algorithm implementation.
```
Finished 'range_search' benchmark with size '0' (25600 keys), '10' iterations and 'dense' keys in 1.7 minutes.

=================================================================================
                                BENCHMARK RESULTS
=================================================================================
Index Structure |      Min      |      Max      |      Avg      |      M Ops/s  |
---------------------------------------------------------------------------------
ART             |      2.1394s  |      2.2206s  |      2.1712s  |      0.0118   |
Trie            |      1.9866s  |      2.0871s  |      2.0365s  |      0.0126   |
M-Trie          |      3.4693s  |      3.6055s  |      3.5485s  |      0.0072   |
Sorted List     |      0.2156s  |      0.2213s  |      0.2190s  |      0.1169   |
RB-Tree         |      2.2345s  |      2.2865s  |      2.2617s  |      0.0113   |



Finished 'range_search' benchmark with size '0' (25600 keys), '10' iterations and 'sparse' keys in 42.4 minutes.

=================================================================================
                                BENCHMARK RESULTS
=================================================================================
Index Structure |      Min      |      Max      |      Avg      |      M Ops/s  |
---------------------------------------------------------------------------------
ART             |     10.0702s  |     10.6519s  |     10.3182s  |      0.0025   |
Trie            |    168.8378s  |    170.6308s  |    169.7028s  |      0.0002   |
M-Trie          |     69.4645s  |     70.8718s  |     70.0710s  |      0.0004   |
Sorted List     |      0.3371s  |      0.3952s  |      0.3483s  |      0.0735   |
RB-Tree         |      3.6889s  |      3.8529s  |      3.7643s  |      0.0068   |
```
