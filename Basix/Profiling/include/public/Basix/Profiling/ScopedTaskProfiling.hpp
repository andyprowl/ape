#pragma once

#include <optional>
#include <tuple>
#include <utility>

namespace basix
{

class TaskProfiler;

template<typename... MetricsCollectors>
class [[nodiscard]] ScopedTaskProfiling
{

public:

    ScopedTaskProfiling()
        : builder{nullptr}
    {
    }

    ScopedTaskProfiling(
        TaskProfiler & builder,
        std::tuple<MetricsCollectors...> collectors)
        : builder{&builder}
        , collectors{std::move(collectors)}
    {
    }

    ScopedTaskProfiling(ScopedTaskProfiling const & rhs) = delete;

    ScopedTaskProfiling(ScopedTaskProfiling && rhs) noexcept
        : builder{std::exchange(rhs.builder, nullptr)}
        , collectors{std::move(rhs.collectors)}
    {
    }

    auto operator = (ScopedTaskProfiling const & rhs)
        -> ScopedTaskProfiling & = delete;

    auto operator = (ScopedTaskProfiling && rhs) noexcept
        -> ScopedTaskProfiling &
    {
        builder = std::exchange(rhs.builder, nullptr);

        collectors = std::move(rhs.collectors);

        return *this;
    }

    ~ScopedTaskProfiling()
    {
        if (builder != nullptr)
        {
            builder->closeCurrentTask(std::move(*collectors));
        }
    }

    template<typename Collector>
    auto getCollector()
        -> Collector &
    {
        return std::get<Collector>(*collectors);
    }

private:

    TaskProfiler * builder;

    std::optional<std::tuple<MetricsCollectors...>> collectors;

};

} // namespace basix
