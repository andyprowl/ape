#pragma once

#include <Ape/GpuResource/GpuResource.hpp>
#include <Ape/GpuResource/TextureDescriptor.hpp>

namespace ape
{

class Texture
{

public:

    explicit Texture(TextureDescriptor descriptor);

    auto getId() const
        -> GpuResource::Id;

    auto bind(int unit) const
        -> void;

    auto getFormat() const
        -> TextureFormat;

    auto getSize() const
        -> Size<int>;

private:

    GpuResource resource;

};

} // namespace ape
