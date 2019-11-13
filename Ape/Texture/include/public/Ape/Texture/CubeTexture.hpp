#pragma once

#include <Ape/Texture/CubeTextureDescriptor.hpp>
#include <Ape/Texture/TextureWrapping.hpp>

#include <Ape/GpuResource/GpuResource.hpp>

namespace ape
{

enum class TextureStorageType;

class CubeTexture
{

public:

    explicit CubeTexture(CubeTextureDescriptor const & descriptor);

    auto getId() const
        -> GpuResource::Id;

    auto activate(int unit) const
        -> void;

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

private:

    GpuResource resource;

};

} // namespace ape
