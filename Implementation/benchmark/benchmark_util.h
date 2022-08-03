#pragma once

#include <string>
#include <cmath>

inline std::string GetDoubleOffset(const double n)
{
    const int p = static_cast<int>(n);
    const int digits = p == 0 ? 1 : static_cast<int>(log10(p)) + 1;

    std::string s;

    for (int i = 0; i < 7 - digits; ++i)
        s.append(" ");

    return s;
}

inline std::string GetIntOffset(const double n)
{
    const int p = static_cast<int>(n);
    const int digits = p == 0 ? 1 : static_cast<int>(log10(p)) + 1;

    std::string s;

    for (int i = 0; i < 9 - digits; ++i)
        s.append(" ");

    return s;
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
