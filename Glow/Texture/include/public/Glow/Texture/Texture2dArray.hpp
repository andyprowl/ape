#pragma once

#include <Glow/GpuResource/GpuResource.hpp>

#include <string_view>

namespace glow
{

class TextureArray2dDescriptor;

class Texture2dArray
{

public:

    explicit Texture2dArray(TextureArray2dDescriptor const & descriptor);

    Texture2dArray(
        TextureArray2dDescriptor const & descriptor,
        bool createMipmap,
        std::string_view label);

    auto getId() const
        -> GpuResource::Id;

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

};

} // namespace glow
