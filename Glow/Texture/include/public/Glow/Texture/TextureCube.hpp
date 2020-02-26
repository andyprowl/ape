#pragma once

#include <Glow/Texture/TextureCubeDescriptor.hpp>
#include <Glow/Texture/TextureWrapping.hpp>

#include <Glow/GpuResource/GpuResource.hpp>

namespace glow
{

enum class TextureStorageType;

class TextureCube
{

public:

    explicit TextureCube(TextureCubeDescriptor const & descriptor);

    TextureCube(
        TextureCubeDescriptor const & descriptor,
        bool createMipmap,
        std::string_view label);

    auto getId() const
        -> GpuResource::Id;

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

    auto generateMipmap()
        -> void;

    auto setLabel(std::string_view label)
        -> void;

private:

    GpuResource resource;

};

} // namespace glow
