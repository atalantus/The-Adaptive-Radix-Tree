#include "hash_table.h"

namespace hash_table
{
    void HashTable::Insert(const uint32_t value)
    {
        hash_table_.insert(value);
    }

    bool HashTable::Find(const uint32_t value) const
    {
        return hash_table_.find(value) != hash_table_.end();
    }
}
