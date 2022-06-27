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