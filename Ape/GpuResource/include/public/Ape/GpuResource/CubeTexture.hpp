#pragma once

#include <Ape/GpuResource/GpuResource.hpp>
#include <Ape/GpuResource/CubeTextureDescriptor.hpp>
#include <Ape/GpuResource/TextureWrapping.hpp>

namespace ape
{

enum class TextureStorageType;

class CubeTexture
{

public:

    CubeTexture(
        CubeTextureDescriptor const & descriptor,
        TextureWrapping wrapping,
        TextureStorageType storageType);

    auto getId() const
        -> GpuResource::Id;

    auto bind(int unit) const
        -> void;

private:

    GpuResource resource;

};

} // namespace ape
