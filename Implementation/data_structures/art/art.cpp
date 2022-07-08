#include "art.h"

namespace art
{
    void Art::Insert(const uint32_t value)
    {
    }

    bool Art::Find(const uint32_t value) const
    {
        Node* node = root_;

        for (uint8_t offset = 0; offset < 32; offset += 8)
        {
            // get next 8 bit of value as partial key
            const uint8_t partial_key = (value >> offset) & 0xFF;

            uint64_t address_value = node->FindChild(partial_key);

            // check if we have a child
            if (address_value == NULL)
                return false;

            // handle lazy expansion
            if (address_value & 0x7ULL)
            {
                // address_value is actual full key value instead of address
                // (key value stored at high 32 bits)
                uint32_t full_key_value = (address_value & (0xFFFFFFFFULL << 32) >> 32);
                if (full_key_value == value)
                    return true;
                // value does not exist
                return false;
            }

            // go to next node
            node = (Node*)address_value;
        }

        // compared full value so it exists
        // (since this ART is only for storing 32 bit keys without path compression
        //  or lazy expansion, leaf nodes are not needed)
        return true;
    }

    std::vector<uint32_t> Art::FindRange(const uint32_t from, const uint32_t to) const
    {
        auto result = std::vector<uint32_t>();

        // TODO: Range search

        return result;
    }
}
