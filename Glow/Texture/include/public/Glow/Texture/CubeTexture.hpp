#pragma once

#include <Glow/Texture/CubeTextureDescriptor.hpp>
#include <Glow/Texture/TextureWrapping.hpp>

#include <Glow/GpuResource/GpuResource.hpp>

namespace glow
{

enum class TextureStorageType;

class CubeTexture
{

public:

    explicit CubeTexture(CubeTextureDescriptor const & descriptor);

    CubeTexture(CubeTextureDescriptor const & descriptor, std::string_view label);

    auto getId() const
        -> GpuResource::Id;

    auto activate(int unit) const
        -> void;

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

    auto setLabel(std::string_view label)
        -> void;

private:

    GpuResource resource;

};

} // namespace glow
