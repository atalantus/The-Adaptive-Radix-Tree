#include "trie.h"

namespace trie {
    void Trie::Insert(const uint32_t value) {
        // current node index (0th to 27th bit)
        uint32_t node_index{0};

        for (int offset = 0; offset < 32; offset += 4) {
            // get next 4 bit of value as comparison key
            const uint32_t comparison_key = (value >> offset) & 0xF;

            // calculate trie key
            uint32_t trie_key = (comparison_key << 28) | node_index;

            if (!node_map_.contains(trie_key)) {
                uint32_t next_node_index = node_map_.size() + 1;

                // create new node on search path
                node_map_.insert({trie_key, next_node_index});

                node_index = next_node_index;
            } else {
                // go to next node
                node_index = node_map_.find(trie_key)->second;
            }
        }
    }

    bool Trie::Find(const uint32_t value) const {
        // current node index (0th to 27th bit)
        uint32_t node_index{0};

        for (int offset = 0; offset < 32; offset += 4) {
            // get next 4 bit of value as comparison key
            const uint32_t comparison_key = (value >> offset) & 0xF;

            // calculate trie key
            uint32_t trie_key = (comparison_key << 28) | node_index;

            if (!node_map_.contains(trie_key)) {
                // there was no node for this comparison key
                // -> value doesn't exist
                return false;
            }

            // go to next node
            node_index = node_map_.find(trie_key)->second;
        }

        // compared full value so it exists
        return true;
    }

    std::vector<uint32_t> Trie::FindRange(const uint32_t from, const uint32_t to)
    {
	    auto result = std::vector<uint32_t>();

        // TODO: Range search

        return result;
    }
}