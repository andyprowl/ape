#pragma once

#include <Glow/GpuResource/GpuResource.hpp>

#include <cstdint>
#include <optional>

namespace glow
{

class Query
{

public:

    auto getId() const
        -> GpuResource::Id;

    auto isResultAvailable() const
        -> bool;

    auto waitForResultAndFetch() const
        -> std::uint64_t;

    auto fetchResultIfAvailable() const
        -> std::optional<std::uint64_t>;

protected:
    
    Query();

protected:

    GpuResource resource;

};

} // namespace glow
