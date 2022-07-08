# ART Implementation (C++)

## Data Structures
### ART
ART Implementation for 64 bit Systems storing 32 bit keys without path compression. 
(This exact implementation uses SSE SIMD from x86-64 and was compiled with MSVC++)

**Notes:**
- Without path compression all nodes only store a 3 byte header ()
- SIMD comparison for searching in Node16 (x86-64 MSVC++ specific)
- Multi-value leaves using pointer tagging (64 bit architecture specific)
### Trie
### MTrie
### FTrie
### RBTree
### Hash Table