#pragma once

#include <Ape/Rendering/GpuProfiling/GpuTimeMetrics.hpp>
#include <Ape/Rendering/GpuProfiling/ElapsedTimeQueryPool.hpp>

#include <memory>

namespace ape
{

class GpuElapsedTimeMetricsCollector
{

public:

    using Nanoseconds = GpuTimeMetrics::Nanoseconds;

public:

    explicit GpuElapsedTimeMetricsCollector(ElapsedTimeQueryPool & queryPool)
        : queryPool{&queryPool}
        , metrics{std::make_unique<GpuTimeMetrics>()}
        , registration{beginQuery()}
    {
    }

    auto extractMetrics()
        -> std::unique_ptr<GpuTimeMetrics>
    {
        endQuery();

        return std::move(metrics);
    }

private:

    auto beginQuery()
        -> ElapsedTimeQueryPool::QueryRegistration
    {
        auto const receiver = [m = metrics.get()] (Nanoseconds const elapsedTime)
        {
            m->duration = elapsedTime;
        };

        return queryPool->beginQuery(receiver);
    }

    auto endQuery()
        -> void
    {
        queryPool->endQuery(registration);
    }

private:

    ElapsedTimeQueryPool * queryPool;

    std::unique_ptr<GpuTimeMetrics> metrics;

    ElapsedTimeQueryPool::QueryRegistration registration;

};

} // namespace ape