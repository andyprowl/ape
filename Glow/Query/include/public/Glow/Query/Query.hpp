#pragma once

#include <Glow/GpuResource/GpuResource.hpp>

#include <cstdint>
#include <optional>

namespace glow
{

class Query
{

public:

    using ResultType = std::uint64_t;

public:

    auto getId() const
        -> GpuResource::Id;

    auto isResultAvailable() const
        -> bool;

    auto waitForResultAndFetch() const
        -> ResultType;

    auto fetchResultIfAvailable() const
        -> std::optional<ResultType>;

protected:
    
    Query();

protected:

    GpuResource resource;

};

} // namespace glow
