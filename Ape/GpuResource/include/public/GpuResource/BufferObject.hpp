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

protected:

    GpuResource resource;

};

} // namespace ape
