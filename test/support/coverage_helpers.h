#pragma once

#include <string>
#include <vector>

#include "Thread.h"

inline bool sentContains(const Thread& thread, const std::string& needle)
{
    for (const auto& msg : thread.sentMessages())
    {
        if (msg.find(needle) != std::string::npos)
            return true;
    }
    return false;
}

inline bool sentEquals(const Thread& thread, const std::string& value)
{
    for (const auto& msg : thread.sentMessages())
    {
        if (msg == value)
            return true;
    }
    return false;
}

inline std::size_t sentCount(const Thread& thread, const std::string& value)
{
    std::size_t count = 0;
    for (const auto& msg : thread.sentMessages())
    {
        if (msg == value)
            ++count;
    }
    return count;
}

inline void queueRecvAll(Thread& thread, const std::vector<std::string>& messages)
{
    for (const auto& message : messages)
        thread.pushRecv(message);
}
