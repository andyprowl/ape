#pragma once

#include <Glow/Texture/TextureDescriptor.hpp>
#include <Glow/Texture/TextureWrapping.hpp>

#include <Glow/GpuResource/GpuResource.hpp>

#include <string_view>

namespace glow
{

class TextureSwizzleMask;

class Texture
{

public:

    explicit Texture(TextureDescriptor const & descriptor);

    Texture(TextureDescriptor const & descriptor, bool createMipmap, std::string_view label);

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

    auto setSwizzleMask(TextureSwizzleMask const & mask)
        -> void;

    auto generateMipmap()
        -> void;

    auto setLabel(std::string_view label)
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
