#pragma once

#include <GpuResource/GpuResource.hpp>

namespace ape
{

class BufferObject
{

protected:
    
    explicit BufferObject(GpuResource resource)
        : resource{std::move(resource)}
    {
    }

    auto isValid() const
        -> bool
    {
        return (resource.get() != 0);
    }

protected:

    GpuResource resource;

};

} // namespace ape
