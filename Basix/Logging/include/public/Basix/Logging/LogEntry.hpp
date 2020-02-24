#pragma once

#include <Basix/Logging/Severity.hpp>

#include <chrono>

namespace basix
{

class LogEntry
{

public:
    
    using Clock = std::chrono::system_clock;

    using TimePoint = Clock::time_point;

public:

    explicit LogEntry(Severity severity)
        : severity{severity}
        , time{Clock::now()}
    {
    }

    auto getSeverity() const
        -> Severity
    {
        return severity;
    }

    auto getTime() const
        -> TimePoint
    {
        return time;
    }
    
private:

    Severity severity;

    TimePoint time;

};

} // namespace basix
