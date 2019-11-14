#pragma once

#include <Glow/GpuResource/GpuResource.hpp>

namespace glow
{

class BufferObject
{

public:

    auto getId() const
        -> GpuResource::Id
    {
        return resource.get();
    }

    auto isValid() const
        -> bool
    {
        return (getId() != 0);
    }

protected:
    
    explicit BufferObject(GpuResource resource)
        : resource{std::move(resource)}
    {
    }

protected:

    GpuResource resource;

};

} // namespace glow
