#pragma once

#include <cstdint>
#include <vector>
#include "node/node.h"

namespace art
{
    class Art
    {
    public:
        Art() : root_{}
        {
        }

        /**
         * Inserts a 32 bit value into the ART.
         *
         * @param value the value to insert
         */
        void Insert(uint32_t value);

        /**
         * Checks if a 32 bit value exists in the ART.
         *
         * @param value the value to search for
         * @return true if the value exists otherwise false
         */
        bool Find(uint32_t value) const;

        /**
         * Finds all 32 bit values in the ART within a given range.
         *
         * @param from the value to search from (inclusive)
         * @param to the value to search to (inclusive)
         * @return vector of values sorted in ascending order
         */
        std::vector<uint32_t> FindRange(uint32_t from, uint32_t to) const;

    private:
        node::Node root_;
    };
}
