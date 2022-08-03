#pragma once

#include <string>
#include <cmath>

inline std::string FormatTime(const double n, const bool unit)
{
    const int p = static_cast<int>(n);
    const int digits = p == 0 ? 1 : static_cast<int>(log10(p)) + 1;

    std::stringstream s;

    for (int i = 0; i < 7 - digits; ++i)
        s << " ";

    s << std::fixed << std::setprecision(4) << n << (unit ? "s" : "") << "\t|";

    return s.str();
}

inline std::string FormatMemory(const uint32_t n)
{
    const std::string n_str = std::to_string(n);
    const size_t digits = n_str.length();

    std::stringstream s;

    for (int i = 0; i < 13 - digits; ++i)
        s << " ";

    for (size_t i = 1; i <= digits; ++i)
    {
        s << n_str[i - 1];
        if (i < digits && (digits - i) % 3 == 0) s << ".";
    }

    s << " byte\t|";

    return s.str();
}

inline char* GetCmdArg(char** begin, char** end, const std::string& arg)
{
    char** itr = std::find(begin, end, arg);
    if (itr != end && ++itr != end)
        return *itr;
    return nullptr;
}

inline bool CmdArgExists(char** begin, char** end, const std::string& arg)
{
    return std::find(begin, end, arg) != end;
}
