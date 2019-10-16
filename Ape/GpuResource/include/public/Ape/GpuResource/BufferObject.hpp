#pragma once

#include <Ape/GpuResource/GpuResource.hpp>

namespace ape
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

} // namespace ape
