#pragma once

#include <cstdint>
#include <unordered_map>

namespace htrie
{
    class HTrie
    {
    public:
        HTrie() : node_map_{{}}
        {
        }

        void Insert(uint32_t value);

        bool Find(uint32_t value) const;

    private:
        /**
         * We store the complete trie in a single hash-map.
         *
         * The key is a 32 bit integer where the first 24 bits (0th up to 23th)
         * represent the current node index and the last 8 bits represent the
         * current symbol used (this means the trie implementation has a span of 8).
         *
         * We can differentiate a maximum of 2^24 or approx. 16.7 million
         * different nodes which is enough for storing all possible 32 bit integer values.
         *
         * Since the trie will have height 3 (the last byte indicated stored using a special index) we will need a maximum of 256^3-1
         *
         * The value is again a 32 bit integer with either the first 24 bits
         * representing the node index this key is referring to or .
         */
        std::unordered_map<uint64_t, uint32_t> node_map_;
    };
}
