#pragma once

#include <Glow/Texture/Texture2dDescriptor.hpp>
#include <Glow/Texture/TextureWrapping.hpp>

#include <Glow/GpuResource/GpuResource.hpp>

#include <string_view>

namespace glow
{

class Image2d;
class TextureSwizzleMask;

class Texture2d
{

public:

    explicit Texture2d(Texture2dDescriptor const & descriptor);

    Texture2d(Texture2dDescriptor const & descriptor, std::string_view label);

    auto getId() const
        -> GpuResource::Id;

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

    auto getDescriptor() const
        -> Texture2dDescriptor const &;

    auto setImage(Image2d const & image, bool createMipmap)
        -> void;

    auto generateMipmap()
        -> void;

    auto setSwizzleMask(TextureSwizzleMask const & mask)
        -> void;

    auto setLabel(std::string_view label)
        -> void;

private:

    GpuResource resource;

    Texture2dDescriptor descriptor;

};

} // namespace glow
