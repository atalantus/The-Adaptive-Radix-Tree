# ART Implementation (C++)

## Data Structures
### ART
**ART Implementation for 64 bit architectures storing 32 bit keys without path compression.**

The exact implementation uses SSE2 SIMD from x86-64 and was compiled and tested with MSVC++ and GCC-11.

**Notes:**
- Without path compression all nodes only store a 2 byte header (1 Byte node type, 1 Byte number of non-null children)
- SIMD comparison for searching in Node16 (x86-64 specific)
- Multi-value leaves using pointer tagging (64 bit architecture specific)
### Trie
### MTrie
### FTrie
### RBTree
### Hash Table