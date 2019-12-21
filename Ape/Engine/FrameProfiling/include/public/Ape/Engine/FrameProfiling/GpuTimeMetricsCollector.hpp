#pragma once

#include <Ape/Engine/FrameProfiling/GpuTimeQueryType.hpp>
#include <Ape/Engine/FrameProfiling/GpuElapsedTimeMetricsCollector.hpp>
#include <Ape/Engine/FrameProfiling/GpuTimestampMetricsCollector.hpp>

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
