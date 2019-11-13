#pragma once

#include <Ape/Texture/TextureDescriptor.hpp>
#include <Ape/Texture/TextureWrapping.hpp>

#include <Ape/GpuResource/GpuResource.hpp>

namespace ape
{

enum class TextureStorageType;

class TextureSwizzleMask;

class Texture
{

public:

    explicit Texture(TextureDescriptor const & descriptor);

    auto getId() const
        -> GpuResource::Id;

    auto activate(int unit) const
        -> void;

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

    auto getImageFormat() const
        -> TextureImageFormat;

    auto getInternalFormat() const
        -> TextureInternalFormat;

    auto getSize() const
        -> Size<int>;

    auto setSize(Size<int> const & newSize)
        -> void;

    auto setSwizzleMask(TextureSwizzleMask const & mask)
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
