#pragma once

#include <Ape/GpuResource/GpuResource.hpp>
#include <Ape/GpuResource/TextureDescriptor.hpp>
#include <Ape/GpuResource/TextureWrapping.hpp>

namespace ape
{

enum class TextureStorageType;

class Texture
{

public:

    explicit Texture(TextureDescriptor const & descriptor);

    auto getId() const
        -> GpuResource::Id;

    auto bind(int unit) const
        -> void;

    auto getImageFormat() const
        -> TextureImageFormat;

    auto getInternalFormat() const
        -> TextureInternalFormat;

    auto getSize() const
        -> Size<int>;

    auto setSize(Size<int> const & newSize)
        -> void;

private:

    auto recreateMutableStorage()
        -> void;

private:

    GpuResource resource;

    Size<int> size;

    TextureImageFormat imageFormat;

    TextureInternalFormat internalFormat;

    PixelType pixelType;

    TextureWrapping wrapping;

};

} // namespace ape
