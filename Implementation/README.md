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
- Deriving from an abstract base class and calling Node functions via dynamic dispatch was faster than manual switch 
case with static_cast while introducing a memory overhead of 8 byte for each node instance which is the extra pointer 
to the node-specific vtable (only one per class so small constant space).
(See branch [`polymorphism_comparison`](https://github.com/atalantus/The-Adaptive-Radix-Tree/tree/polymorphism_comparison) 
for comparison implementation and [this blog post](https://eli.thegreenplace.net/2013/12/05/the-cost-of-dynamic-virtual-calls-vs-static-crtp-dispatch-in-c) as a great resource on the topic in general.)
- Without path compression all nodes only store a 2 byte header (1 Byte node type, 1 Byte number of non-null children)
The node sizes are as follows (+ 8 byte vtable pointer, but since we don't need to store the node type explicitly anymore we have a total overhead of 7 byte):
  - Node4: 2+4+4*8 = 38 byte (padded to 40 byte)
  - Node16: 2+16+16*8 = 146 byte (padded to 152 byte)
  - Node48: 2+256+48*8 = 642 byte (padded to 648 byte)
  - Node48: 2+256*8 = 2050 byte (padded to 2056 byte)
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

### Memory Consumption
Total memory consumed in bytes when storing uniformly (in the population) distributed keys.

```
Finished 'insert' benchmark with size '1' (65000 keys), '200' iterations and 'dense' keys in 0.2 minutes.

=================================================================================================================
                                        MEMORY BENCHMARK RESULTS
=================================================================================================================
Index Structure |         Min           |         Max           |         Avg           |         Med           |
-----------------------------------------------------------------------------------------------------------------
ART             |         524.368 byte  |         524.368 byte  |         524.368 byte  |         524.368 byte  |
ART (EXP)       |         526.440 byte  |         526.440 byte  |         526.440 byte  |         526.440 byte  |
Trie            |         532.512 byte  |         532.512 byte  |         532.512 byte  |         532.512 byte  |
M-Trie          |       1.989.160 byte  |       2.013.304 byte  |       2.001.644 byte  |       2.001.640 byte  |
Sorted List     |         260.063 byte  |         260.063 byte  |         260.063 byte  |         260.063 byte  |
Hash-Table      |       2.028.911 byte  |       2.040.983 byte  |       2.035.153 byte  |       2.035.151 byte  |
RB-Tree         |       1.306.992 byte  |       1.323.088 byte  |       1.315.314 byte  |       1.315.312 byte  |



Finished 'insert' benchmark with size '1' (65000 keys), '200' iterations and 'sparse' keys in 0.8 minutes.

=================================================================================================================
                                        MEMORY BENCHMARK RESULTS
=================================================================================================================
Index Structure |         Min           |         Max           |         Avg           |         Med           |
-----------------------------------------------------------------------------------------------------------------
ART             |       1.234.088 byte  |       1.251.720 byte  |       1.243.340 byte  |       1.243.300 byte  |
ART (EXP)       |       1.506.328 byte  |       1.529.904 byte  |       1.519.459 byte  |       1.519.380 byte  |
Trie            |     219.876.504 byte  |     220.844.128 byte  |     220.382.849 byte  |     220.386.216 byte  |
M-Trie          |      15.005.176 byte  |      15.057.480 byte  |      15.032.524 byte  |      15.032.704 byte  |
Sorted List     |         260.063 byte  |         260.063 byte  |         260.063 byte  |         260.063 byte  |
Hash-Table      |       2.608.631 byte  |       2.608.703 byte  |       2.608.692 byte  |       2.608.703 byte  |
RB-Tree         |       2.079.952 byte  |       2.080.048 byte  |       2.080.033 byte  |       2.080.048 byte  |



Finished 'insert' benchmark with size '2' (16000000 keys), '5' iterations and 'dense' keys in 5.5 minutes.

=================================================================================================================
                                        MEMORY BENCHMARK RESULTS
=================================================================================================================
Index Structure |         Min           |         Max           |         Avg           |         Med           |
-----------------------------------------------------------------------------------------------------------------
ART             |     129.004.416 byte  |     129.004.416 byte  |     129.004.416 byte  |     129.004.416 byte  |
ART (EXP)       |     129.506.400 byte  |     129.506.400 byte  |     129.506.400 byte  |     129.506.400 byte  |
Trie            |     130.011.792 byte  |     130.011.792 byte  |     130.011.792 byte  |     130.011.792 byte  |
M-Trie          |     492.459.208 byte  |     492.563.128 byte  |     492.511.691 byte  |     492.506.248 byte  |
Sorted List     |      64.000.063 byte  |      64.000.063 byte  |      64.000.063 byte  |      64.000.063 byte  |
Hash-Table      |     511.151.375 byte  |     511.203.335 byte  |     511.177.616 byte  |     511.174.895 byte  |
RB-Tree         |     323.621.104 byte  |     323.690.384 byte  |     323.656.092 byte  |     323.652.464 byte  |



Finished 'insert' benchmark with size '2' (16000000 keys), '5' iterations and 'sparse' keys in 57.5 minutes.

=================================================================================================================
                                        MEMORY BENCHMARK RESULTS
=================================================================================================================
Index Structure |         Min           |         Max           |         Avg           |         Med           |
-----------------------------------------------------------------------------------------------------------------
ART             |     306.137.840 byte  |     306.174.128 byte  |     306.156.707 byte  |     306.159.984 byte  |
ART (EXP)       |     372.432.560 byte  |     372.520.008 byte  |     372.468.604 byte  |     372.455.992 byte  |
Trie            |  21.501.912.720 byte  |  21.505.319.088 byte  |  21.503.208.548 byte  |  21.503.075.112 byte  |
M-Trie          |   1.928.844.024 byte  |   1.929.025.528 byte  |   1.928.910.811 byte  |   1.928.903.320 byte  |
Sorted List     |      64.000.063 byte  |      64.000.063 byte  |      64.000.063 byte  |      64.000.063 byte  |
Hash-Table      |     651.718.679 byte  |     651.728.039 byte  |     651.723.210 byte  |     651.723.071 byte  |
RB-Tree         |     511.044.176 byte  |     511.056.656 byte  |     511.050.217 byte  |     511.050.032 byte  |
```

### Insert
Insertion of uniformly (in the population) distributed keys.

H-Trie results are currently omitted due to implementation errors.
```
Finished 'insert' benchmark with size '1' (65000 keys), '200' iterations and 'dense' keys in 0.2 minutes.

=================================================================================================================
                                        PERFORMANCE BENCHMARK RESULTS
=================================================================================================================
Index Structure |      Min      |      Max      |      Avg      |      Med      | M Ops/s (Avg) | M Ops/s (Med) |
-----------------------------------------------------------------------------------------------------------------
ART             |      0.0017s  |      0.0023s  |      0.0020s  |      0.0020s  |     32.9753   |     32.9656   |
ART (EXP)       |      0.0017s  |      0.0023s  |      0.0019s  |      0.0019s  |     33.8452   |     33.4508   |
Trie            |      0.0004s  |      0.0007s  |      0.0006s  |      0.0006s  |    117.9261   |    114.4870   |
M-Trie          |      0.0167s  |      0.0185s  |      0.0177s  |      0.0177s  |      3.6690   |      3.6628   |
Sorted List     |      0.0039s  |      0.0043s  |      0.0040s  |      0.0040s  |     16.0977   |     16.0891   |
Hash-Table      |      0.0038s  |      0.0050s  |      0.0043s  |      0.0044s  |     15.1181   |     14.8463   |
RB-Tree         |      0.0117s  |      0.0131s  |      0.0124s  |      0.0125s  |      5.2396   |      5.1949   |



Finished 'insert' benchmark with size '1' (65000 keys), '200' iterations and 'sparse' keys in 0.8 minutes.

=================================================================================================================
                                        PERFORMANCE BENCHMARK RESULTS
=================================================================================================================
Index Structure |      Min      |      Max      |      Avg      |      Med      | M Ops/s (Avg) | M Ops/s (Med) |
-----------------------------------------------------------------------------------------------------------------
ART             |      0.0026s  |      0.0034s  |      0.0028s  |      0.0027s  |     23.1377   |     24.0038   |
ART (EXP)       |      0.0026s  |      0.0034s  |      0.0027s  |      0.0026s  |     24.0774   |     24.7416   |
Trie            |      0.0375s  |      0.1183s  |      0.0574s  |      0.0424s  |      1.1334   |      1.5327   |
M-Trie          |      0.0436s  |      0.0581s  |      0.0459s  |      0.0444s  |      1.4157   |      1.4648   |
Sorted List     |      0.0037s  |      0.0045s  |      0.0038s  |      0.0038s  |     16.9576   |     17.0030   |
Hash-Table      |      0.0049s  |      0.0082s  |      0.0053s  |      0.0051s  |     12.3623   |     12.7668   |
RB-Tree         |      0.0142s  |      0.0209s  |      0.0147s  |      0.0145s  |      4.4155   |      4.4678   |



Finished 'insert' benchmark with size '2' (16000000 keys), '20' iterations and 'dense' keys in 18.6 minutes.

=================================================================================================================
                                        PERFORMANCE BENCHMARK RESULTS
=================================================================================================================
Index Structure |      Min      |      Max      |      Avg      |      Med      | M Ops/s (Avg) | M Ops/s (Med) |
-----------------------------------------------------------------------------------------------------------------
ART             |      1.3124s  |      1.4317s  |      1.3593s  |      1.3523s  |     11.7704   |     11.8317   |
ART (EXP)       |      1.3736s  |      1.5648s  |      1.4436s  |      1.4409s  |     11.0836   |     11.1045   |
Trie            |      0.5759s  |      0.8185s  |      0.6387s  |      0.6270s  |     25.0496   |     25.5166   |
M-Trie          |     22.1451s  |     27.1949s  |     23.4499s  |     22.6882s  |      0.6823   |      0.7052   |
Sorted List     |      1.4731s  |      1.5633s  |      1.4921s  |      1.4823s  |     10.7228   |     10.7939   |
Hash-Table      |      4.1505s  |      4.8821s  |      4.3259s  |      4.2577s  |      3.6987   |      3.7579   |
RB-Tree         |     16.0015s  |     19.8165s  |     16.9046s  |     16.3841s  |      0.9465   |      0.9766   |



Finished 'insert' benchmark with size '2' (16000000 keys), '20' iterations and 'sparse' keys in 28.0 minutes.

=================================================================================================================
                                        PERFORMANCE BENCHMARK RESULTS
=================================================================================================================
Index Structure |      Min      |      Max      |      Avg      |      Med      | M Ops/s (Avg) | M Ops/s (Med) |
-----------------------------------------------------------------------------------------------------------------
ART             |      1.8973s  |      2.1820s  |      2.0339s  |      2.0228s  |      7.8668   |      7.9097   |
ART (EXP)       |      2.0007s  |      2.2460s  |      2.1367s  |      2.1549s  |      7.4882   |      7.4249   |
M-Trie          |     30.8372s  |     32.4310s  |     31.8199s  |     31.9504s  |      0.5028   |      0.5008   |
Sorted List     |      1.4364s  |      1.4563s  |      1.4420s  |      1.4424s  |     11.0958   |     11.0924   |
Hash-Table      |      4.9472s  |      5.3916s  |      5.1241s  |      5.1445s  |      3.1225   |      3.1101   |
RB-Tree         |     18.1814s  |     19.6065s  |     18.9986s  |     19.1375s  |      0.8422   |      0.8361   |
```

### Search
Positive single-point lookups of uniformly (in the population) distributed keys.

H-Trie results are currently omitted due to implementation errors.
```
Finished 'search' benchmark with size '1' (65000 keys), '200' iterations and 'dense' keys in 0.3 minutes.

=================================================================================================================
                                        PERFORMANCE BENCHMARK RESULTS
=================================================================================================================
Index Structure |      Min      |      Max      |      Avg      |      Med      | M Ops/s (Avg) | M Ops/s (Med) |
-----------------------------------------------------------------------------------------------------------------
ART             |      0.0011s  |      0.0012s  |      0.0011s  |      0.0011s  |     57.5297   |     57.8987   |
ART (EXP)       |      0.0010s  |      0.0012s  |      0.0011s  |      0.0011s  |     60.1518   |     60.5440   |
Trie            |      0.0003s  |      0.0004s  |      0.0004s  |      0.0003s  |    185.4470   |    186.6475   |
M-Trie          |      0.0174s  |      0.0189s  |      0.0178s  |      0.0178s  |      3.6506   |      3.6536   |
Sorted List     |      0.0048s  |      0.0051s  |      0.0049s  |      0.0049s  |     13.3566   |     13.3695   |
Hash-Table      |      0.0006s  |      0.0008s  |      0.0006s  |      0.0006s  |    102.8004   |    103.5444   |
RB-Tree         |      0.0100s  |      0.0107s  |      0.0102s  |      0.0102s  |      6.3623   |      6.3721   |



Finished 'search' benchmark with size '1' (65000 keys), '200' iterations and 'sparse' keys in 1.0 minutes.

=================================================================================================================
                                        PERFORMANCE BENCHMARK RESULTS
=================================================================================================================
Index Structure |      Min      |      Max      |      Avg      |      Med      | M Ops/s (Avg) | M Ops/s (Med) |
-----------------------------------------------------------------------------------------------------------------
ART             |      0.0017s  |      0.0019s  |      0.0017s  |      0.0017s  |     37.4683   |     37.6049   |
ART (EXP)       |      0.0017s  |      0.0019s  |      0.0018s  |      0.0017s  |     37.1417   |     37.3188   |
Trie            |      0.0047s  |      0.0062s  |      0.0051s  |      0.0050s  |     12.7904   |     12.8862   |
M-Trie          |      0.0442s  |      0.0540s  |      0.0454s  |      0.0452s  |      1.4332   |      1.4384   |
Sorted List     |      0.0050s  |      0.0054s  |      0.0052s  |      0.0052s  |     12.5847   |     12.5975   |
Hash-Table      |      0.0011s  |      0.0013s  |      0.0012s  |      0.0012s  |     54.8181   |     55.2533   |
RB-Tree         |      0.0113s  |      0.0128s  |      0.0116s  |      0.0116s  |      5.5884   |      5.6167   |



Finished 'search' benchmark with size '2' (16000000 keys), '20' iterations and 'dense' keys in 34.3 minutes.

=================================================================================================================
                                        PERFORMANCE BENCHMARK RESULTS
=================================================================================================================
Index Structure |      Min      |      Max      |      Avg      |      Med      | M Ops/s (Avg) | M Ops/s (Med) |
-----------------------------------------------------------------------------------------------------------------
ART             |      1.1238s  |      1.1585s  |      1.1381s  |      1.1323s  |     14.0582   |     14.1308   |
ART (EXP)       |      1.1105s  |      1.1508s  |      1.1325s  |      1.1367s  |     14.1283   |     14.0759   |
Trie            |      0.4570s  |      0.4909s  |      0.4780s  |      0.4786s  |     33.4756   |     33.4302   |
M-Trie          |     23.6882s  |     24.2110s  |     23.9046s  |     23.8902s  |      0.6693   |      0.6697   |
Sorted List     |      4.4321s  |      4.5423s  |      4.4697s  |      4.4631s  |      3.5797   |      3.5850   |
Hash-Table      |      0.8410s  |      0.8860s  |      0.8613s  |      0.8651s  |     18.5767   |     18.4950   |
RB-Tree         |     16.6185s  |     17.1354s  |     16.8503s  |     16.8426s  |      0.9495   |      0.9500   |



Finished 'search' benchmark with size '2' (16000000 keys), '20' iterations and 'sparse' keys in 49.0 minutes.

=================================================================================================================
                                        PERFORMANCE BENCHMARK RESULTS
=================================================================================================================
Index Structure |      Min      |      Max      |      Avg      |      Med      | M Ops/s (Avg) | M Ops/s (Med) |
-----------------------------------------------------------------------------------------------------------------
ART             |      1.7507s  |      1.8398s  |      1.7934s  |      1.7969s  |      8.9218   |      8.9043   |
ART (EXP)       |      1.7950s  |      1.8816s  |      1.8432s  |      1.8476s  |      8.6808   |      8.6598   |
M-Trie          |     31.6994s  |     32.9312s  |     31.9961s  |     31.9692s  |      0.5001   |      0.5005   |
Sorted List     |      4.9285s  |      5.2764s  |      5.0069s  |      4.9908s  |      3.1956   |      3.2059   |
Hash-Table      |      1.1140s  |      1.1876s  |      1.1455s  |      1.1499s  |     13.9677   |     13.9142   |
RB-Tree         |     19.1209s  |     20.6546s  |     19.4263s  |     19.4078s  |      0.8236   |      0.8244   |
```

### Range Search
Index structures not supporting range queries were omitted (H-Trie, Hash-Table).

Executes 25600 range queries each with its lower and upper bound key uniformly (in the population) distributed.

Note that currently the results for ART, Trie and M-Trie may be worse than actuality because of inperformant
algorithm implementation.
```
Finished 'range_search' benchmark with size '1' (65000 keys), '5' iterations and 'dense' keys in 7.4 minutes.

=================================================================================================================
                                        PERFORMANCE BENCHMARK RESULTS
=================================================================================================================
Index Structure |      Min      |      Max      |      Avg      |      Med      | M Ops/s (Avg) | M Ops/s (Med) |
-----------------------------------------------------------------------------------------------------------------
ART             |     17.8223s  |     19.1190s  |     18.1637s  |     17.9281s  |      0.0036   |      0.0036   |
ART (EXP)       |     17.7125s  |     18.2990s  |     17.9042s  |     17.7660s  |      0.0036   |      0.0037   |
Trie            |     12.4566s  |     12.8982s  |     12.6114s  |     12.5522s  |      0.0052   |      0.0052   |
M-Trie          |     23.3375s  |     23.7980s  |     23.5294s  |     23.5073s  |      0.0028   |      0.0028   |
Sorted List     |      1.2009s  |      1.3589s  |      1.2380s  |      1.2073s  |      0.0525   |      0.0538   |
RB-Tree         |     14.9244s  |     15.1223s  |     15.0459s  |     15.0729s  |      0.0043   |      0.0043   |
```
