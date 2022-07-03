#pragma once

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

inline bool IsBigEndian()
{
    union
    {
        uint32_t i;
        char c[4];
    } eint = {0x01020304};

    return eint.c[0] == 1;
}

inline uint32_t SwapEndianess(const uint32_t value)
{
    return (value & 0xFF000000) >> 24
        | (value & 0xFF0000) >> 8
        | (value & 0xFF00) << 8
        | ((unsigned)value & 0xFF) << 24;
}
