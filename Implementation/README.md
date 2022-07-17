# ART Implementation (C++)

## Usage
To run the project compile it on a x86-64 architecture supporting at least SSE2 with either MSVC++ or GCC-11 (or later).

## Data Structures
Various data structures for storing 32 bit keys.

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

## Benchmark Results
The following results were benchmarked on a Intel Core i5-8400 CPU with 2.80GHz base and around 3.80GHz reported frequency during bechmarking with 16GB DDR4-2666MHz RAM.

### Insert
```
```

### Search
```
```

### Range Search
```
```