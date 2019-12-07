#include <Basix/Profiling/ScopedTaskCloser.hpp>

#include <Basix/Profiling/ProfiledTaskBuilder.hpp>

namespace basix
{

ScopedTaskCloser::~ScopedTaskCloser()
{
    if (builder != nullptr)
    {
        builder->closeCurrentTask();
    }
}

} // namespace basix
