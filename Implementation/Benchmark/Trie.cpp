#include "Trie.h"

void Trie::insert(uint32_t value) {
    // current node index (0th to 27th bit)
    uint32_t node_index{0};

    for (int offset = 0; offset < 32; offset += 4) {
        // get next 4 bit of value as comparison key
        uint32_t comparison_key = (value >> offset) & 0xF;

        // calculate trie key
        uint32_t trie_key = (comparison_key << 28) | node_index;

        if (trie.find(trie_key) == trie.end()) {
            uint32_t next_node_index = trie.size() + 1;

            // create new node on search path
            trie.insert({trie_key, next_node_index});

            node_index = next_node_index;
        } else {
            // go to next node
            node_index = trie.find(trie_key)->second;
        }
    }
}

bool Trie::find(uint32_t value) const {
    // current node index (0th to 27th bit)
    uint32_t node_index{0};

    for (int offset = 0; offset < 32; offset += 4) {
        // get next 4 bit of value as comparison key
        uint32_t comparison_key = (value >> offset) & 0xF;

        // calculate trie key
        uint32_t trie_key = (comparison_key << 28) | node_index;

        if (trie.find(trie_key) == trie.end()) {
            // there was no node for this comparison key
            // -> value doesn't exist
            return false;
        } else {
            // go to next node
            node_index = trie.find(trie_key)->second;
        }
    }

    // compared full value so it exists
    return true;
}

std::vector<uint32_t> Trie::find_range(uint32_t from, uint32_t to) const {
    std::vector<uint32_t> result = std::vector<uint32_t>();

    return result;
}

