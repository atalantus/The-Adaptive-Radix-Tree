#pragma once

#include <sstream>
#include <iomanip>
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

inline std::string FormatMemory(const uint64_t n)
{
    const std::string n_str = std::to_string(n);
    const size_t digits = n_str.length();

    std::stringstream s;
    std::string fn_str;

    for (size_t i = 1; i <= digits; ++i)
    {
        fn_str.push_back(n_str[i - 1]);
        if (i < digits && (digits - i) % 3 == 0)
            fn_str.push_back('.');
    }

    for (size_t i = 0; i < 16 - fn_str.size(); ++i)
        s << " ";

    s << fn_str << " byte\t|";

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
