#pragma once

#include <Glow/Texture/Texture2dDescriptor.hpp>
#include <Glow/Texture/TextureWrapping.hpp>

#include <Glow/GpuResource/GpuResource.hpp>

#include <string_view>

namespace glow
{

class TextureSwizzleMask;

class Texture2d
{

public:

    explicit Texture2d(Texture2dDescriptor const & descriptor);

    Texture2d(Texture2dDescriptor const & descriptor, bool createMipmap, std::string_view label);

    auto getId() const
        -> GpuResource::Id;

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

    auto getImageFormat() const
        -> TextureImageFormat;

    auto getInternalFormat() const
        -> TextureInternalFormat;

    auto getSize() const
        -> basix::Size2d<int>;

    auto setSwizzleMask(TextureSwizzleMask const & mask)
        -> void;

    auto generateMipmap()
        -> void;

    auto setLabel(std::string_view label)
        -> void;

private:

    GpuResource resource;

    basix::Size2d<int> size;

    TextureImageFormat imageFormat;

    TextureInternalFormat internalFormat;

    PixelType pixelType;

    TextureWrapping wrapping;

};

} // namespace glow
