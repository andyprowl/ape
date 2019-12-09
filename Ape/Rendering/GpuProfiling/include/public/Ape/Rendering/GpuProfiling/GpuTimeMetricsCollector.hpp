#pragma once

#include <Ape/Rendering/GpuProfiling/GpuTimeQueryType.hpp>
#include <Ape/Rendering/GpuProfiling/GpuElapsedTimeMetricsCollector.hpp>
#include <Ape/Rendering/GpuProfiling/GpuTimestampMetricsCollector.hpp>

namespace ape
{

template<GpuTimeQueryType QueryType>
class GpuTimeMetricsCollector;

template<>
class GpuTimeMetricsCollector<GpuTimeQueryType::timestamp> : public GpuTimestampMetricsCollector
{

public:

    using GpuTimestampMetricsCollector::GpuTimestampMetricsCollector;

};

template<>
class GpuTimeMetricsCollector<GpuTimeQueryType::elapsedTime>
    : public GpuElapsedTimeMetricsCollector
{

public:

    using GpuElapsedTimeMetricsCollector::GpuElapsedTimeMetricsCollector;

};

} // namespace ape
