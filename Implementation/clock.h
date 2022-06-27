#pragma once

#include <chrono>

class Clock
{
public:
    Clock() : t1_{}
    {
    }

    void Start()
    {
        t1_ = std::chrono::high_resolution_clock::now();
    }

    double Stop() const
    {
        const auto t2 = std::chrono::high_resolution_clock::now();

        const std::chrono::duration<double, std::milli> dur{t2 - t1_};

        return dur.count() / 1000.0;
    }

private:
    std::chrono::steady_clock::time_point t1_;
};
