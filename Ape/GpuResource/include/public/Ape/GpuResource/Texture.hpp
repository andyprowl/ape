#pragma once

#include <Ape/GpuResource/GpuResource.hpp>
#include <Ape/GpuResource/TextureDescriptor.hpp>
#include <Ape/GpuResource/TextureWrapping.hpp>

namespace ape
{

class Texture
{

public:

    Texture(TextureDescriptor descriptor, TextureWrapping wrapping);

    auto getId() const
        -> GpuResource::Id;

    auto bind(int unit) const
        -> void;

    auto getFormat() const
        -> TextureFormat;

    auto getSize() const
        -> Size<int>;

    auto setSize(Size<int> const & newSize)
        -> void;

private:

    GpuResource resource;

    Size<int> size;

    TextureFormat format;

    PixelType pixelType;

};

} // namespace ape
