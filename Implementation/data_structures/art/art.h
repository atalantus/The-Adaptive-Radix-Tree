#pragma once

#include <cstdint>
#include <vector>
#include "node.h"

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
        /**
         * Returns true if the address value is actually a full key stored using multi-value lazy expansion.
         */
        static bool IsLazyExpanded(uint64_t address_value);

        /**
         * Compares a full key stored via lazy expansion at address_value with key
         * and returns true if they are the same.
         */
        static bool CmpLazyExpansion(uint64_t address_value, uint32_t key);

    private:
        Node* root_;
    };

    inline bool Art::IsLazyExpanded(const uint64_t address_value)
    {
        return address_value & 0x7ULL;
    }

    inline bool Art::CmpLazyExpansion(const uint64_t address_value, const uint32_t key)
    {
        // address_value is actual full key value instead of address
        // (key value stored at high 32 bits)
        uint32_t full_key_value = (address_value & (0xFFFFFFFFULL << 32) >> 32);
        if (full_key_value == key)
            return true;
        // full key value is different
        return false;
    }
}
