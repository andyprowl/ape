#pragma once

#include <Glow/Texture/TextureDescriptor.hpp>
#include <Glow/Texture/TextureWrapping.hpp>

#include <Glow/GpuResource/GpuResource.hpp>

namespace glow
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
        -> basix::Size<int>;

    auto setSize(basix::Size<int> const & newSize)
        -> void;

    auto setSwizzleMask(TextureSwizzleMask const & mask)
        -> void;

private:

    auto recreateMutableStorage()
        -> void;

private:

    GpuResource resource;

    basix::Size<int> size;

    TextureImageFormat imageFormat;

    TextureInternalFormat internalFormat;

    PixelType pixelType;

    TextureWrapping wrapping;

};

} // namespace glow
