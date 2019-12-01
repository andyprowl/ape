#include <Basix/Profiling/ScopedTaskProfiling.hpp>

#include <Basix/Profiling/TaskProfiler.hpp>

namespace basix
{

auto ScopedTaskProfiling::ProfileCompleter::operator () (TaskProfiler * const profiler) const
    -> void
{
    profiler->recordActiveTaskCompletion();
}

} // namespace basix
