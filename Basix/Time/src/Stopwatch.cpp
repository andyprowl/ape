#include <Basix/Time/Stopwatch.hpp>

namespace basix
{

Stopwatch::Stopwatch()
    : startTime{Clock::now()}
{
}

auto Stopwatch::restart()
    -> void
{
    startTime = Clock::now();
}

auto Stopwatch::getElapsedTime() const
    -> Nanoseconds
{
    using std::chrono::duration_cast;
    
    return duration_cast<Nanoseconds>(Clock::now() - startTime);
}

} // namespace basix
