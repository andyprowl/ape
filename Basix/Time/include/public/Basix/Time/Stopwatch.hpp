#pragma once

#include <chrono>

namespace basix
{

class Stopwatch
{

public:

    using Clock = std::chrono::high_resolution_clock;

    using Nanoseconds = std::chrono::nanoseconds;

public:

    Stopwatch();

    auto restart()
        -> void;

    auto getElapsedTime() const
        -> Nanoseconds;

private:

    Clock::time_point startTime;

};

} // namespace basix
