#pragma once

#include <cstdint>
#include <vector>

class ART {
public:
    void insert(uint32_t value);

    uint32_t find(uint32_t value) const;

    std::vector<uint32_t> find_range(uint32_t from, uint32_t to) const;
};

