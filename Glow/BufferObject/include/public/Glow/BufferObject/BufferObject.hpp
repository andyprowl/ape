#pragma once

#include <Glow/BufferObject/BufferStorageFlags.hpp>

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

    auto createStorage(
        void const * data,
        std::size_t sizeInBytes,
        BufferStorageFlags flags = BufferStorageFlags::none)
        -> void;

protected:
    
    explicit BufferObject(GpuResource resource)
        : resource{std::move(resource)}
    {
    }

protected:

    GpuResource resource;

};

} // namespace glow
