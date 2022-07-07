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

            Node* child = node->FindChild(partial_key);

            if (child == nullptr)
            {
            }
            else
            {
            }
        }

        return false;
    }

    std::vector<uint32_t> Art::FindRange(const uint32_t from, const uint32_t to) const
    {
        auto result = std::vector<uint32_t>();

        // TODO: Range search

        return result;
    }
}
